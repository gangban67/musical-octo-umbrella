# -*- coding: utf-8 -*-
"""实验报告生成器 demo（纯脚本，不调用 AI）
用法：
  py gen.py list                                  # 看题库
  py gen.py list 数据结构与算法 3                  # 看某次实验收录的题目
  py gen.py generate 数据结构与算法 3 "单链表的基本操作" --sid 252226209122 --name 李施贝仑
  py gen.py validate                              # 全库编译 + 跑样例 + 比对输出
  py gen.py new 数据结构与算法 3 "循环链表的合并"    # 生成空题库条目模板
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import unicodedata

from docx import Document
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor
from docx.table import Table
from docx.text.paragraph import Paragraph

HERE = os.path.dirname(os.path.abspath(__file__))


def data_root():
    """打包成 exe 后数据放在 exe 同级的 data 目录，方便更新题库"""
    if getattr(sys, 'frozen', False):
        return os.path.join(os.path.dirname(sys.executable), 'data')
    return HERE


BASE = data_root()
BANK = os.path.join(BASE, 'bank')
CODE = os.path.join(BASE, 'code')
OUT = os.path.join(BASE, 'out')
TEMPLATE = os.path.join(BASE, 'templates', 'official.docx')
KB_FILE = os.path.join(BASE, 'kb', 'topics.json')
SUBJECTS_FILE = os.path.join(BASE, 'kb', 'subjects.json')
GCC = 'gcc'


# ---------------- 题库 ----------------
def resolve_subject(text):
    for name in os.listdir(BANK):
        d = os.path.join(BANK, name)
        if not os.path.isdir(d):
            continue
        idx = load_index(name)
        keys = [name] + idx.get('aliases', [])
        for k in keys:
            if norm(k) == norm(text) or norm(text) in norm(k):
                return name
    raise SystemExit('未找到主题：%s\n可用：%s' % (text, ', '.join(os.listdir(BANK))))


def pick_bank(text):
    """按科目找题库目录；没匹配上就用第一个（目前题库只有一门课）"""
    info = subject_info(text)
    if info and info.get('bank'):
        d = os.path.join(BANK, info['bank'])
        if os.path.isdir(d):
            return info['bank']
    names = [n for n in os.listdir(BANK) if os.path.isdir(os.path.join(BANK, n))]
    nt = norm(text)
    for n in names:
        idx = load_index(n)
        for k in [n] + idx.get('aliases', []):
            if nt and (nt == norm(k) or nt in norm(k)):
                return n
    return names[0] if names else None


def load_index(subject):
    p = os.path.join(BANK, subject, 'index.json')
    with open(p, encoding='utf-8') as f:
        return json.load(f)


def parse_no(text):
    t = norm(str(text))
    t = re.sub(r'(实验|第|次|课)', '', t)
    cn = {'一': 1, '二': 2, '三': 3, '四': 4, '五': 5, '六': 6, '七': 7, '八': 8}
    if t in cn:
        return cn[t]
    if t.isdigit():
        return int(t)
    raise SystemExit('无法识别实验次数：%s' % text)


def session_dir(subject, no):
    d = os.path.join(BANK, subject, '第%02d次' % no)
    return d if os.path.isdir(d) else None


def load_items(d, topic=None):
    if not d:
        return []
    items = []
    for fn in sorted(os.listdir(d)):
        if fn.endswith('.json'):
            with open(os.path.join(d, fn), encoding='utf-8') as f:
                it = json.load(f)
            it['_file'] = fn
            if topic:
                it['topic'] = topic
            items.append(it)
    return items


def load_session(subject, no):
    return load_items(session_dir(subject, no))


def topic_dir(subject, topic):
    if not subject or not topic:
        return None
    d = os.path.join(BANK, subject, topic)
    return d if os.path.isdir(d) else None


def load_topic(subject, topic):
    """按主题目录取题目：题库/<科目>/<主题>/*.json"""
    return load_items(topic_dir(subject, topic), topic)


def bank_topics(subject):
    """题库里已有题目的主题目录，附带上面配置的主题"""
    idx = load_index(subject)
    names = list(idx.get('topics') or [s.get('name') for s in idx.get('sessions', [])])
    for n in sorted(os.listdir(os.path.join(BANK, subject))):
        d = os.path.join(BANK, subject, n)
        if os.path.isdir(d) and n not in names and n != 'kb':
            names.append(n)
    return [n for n in names if topic_dir(subject, n)]


def load_topics():
    """读取主题原理库：kb/topics.json，每个主题含名称、别名与知识点数组"""
    if not os.path.isfile(KB_FILE):
        return []
    with open(KB_FILE, encoding='utf-8') as f:
        return json.load(f)


def topic_names(subject=None):
    """某科目可选的主题；不传科目则返回全部主题"""
    all_names = [t['name'] for t in load_topics()]
    info = subject_info(subject) if subject else None
    if not info:
        return all_names
    allow = {norm(n) for n in info.get('topics', [])}
    return [n for n in all_names if norm(n) in allow]


def load_subjects():
    """科目配置：kb/subjects.json，含显示名、别名、题库目录与可选主题"""
    if not os.path.isfile(SUBJECTS_FILE):
        return []
    with open(SUBJECTS_FILE, encoding='utf-8') as f:
        return json.load(f)


def subject_names():
    return [s['name'] for s in load_subjects()]


def subject_info(text):
    nt = norm(text)
    if not nt:
        return None
    for s in load_subjects():
        if nt in [norm(k) for k in [s['name']] + s.get('aliases', [])]:
            return s
    return None


def find_topic(text, subject=None):
    """按名称或别名找主题；给定科目时只在该科目的主题里找"""
    nt = norm(text)
    info = subject_info(subject) if subject else None
    allow = {norm(n) for n in info.get('topics', [])} if info else None
    for t in load_topics():
        if allow is not None and norm(t['name']) not in allow:
            continue
        if nt in [norm(t['name'])] + [norm(a) for a in t.get('aliases', [])]:
            return t
    return None


def load_kb(subject=None):
    """全部知识点，按 id 索引；每条带上所属主题 topic"""
    names = set(topic_names(subject)) if subject else None
    kb = {}
    for t in load_topics():
        if names is not None and t['name'] not in names:
            continue
        for c in t['concepts']:
            c = dict(c)
            c['topic'] = t['name']
            kb[c['id']] = c
    return kb


def topic_of(item, kb=None):
    """题目所属主题：优先取加载时写上的目录主题，其次看原理引用"""
    if item.get('topic'):
        return item['topic']
    kb = kb or load_kb()
    for r in item.get('principle_refs', []):
        if r in kb:
            return kb[r]['topic']
    return None


def resolve_principle(item, kb):
    """返回 [(概念标题, [要点...])]，并把本题差异并入最后一组"""
    groups = []
    for ref in item.get('principle_refs', []):
        c = kb.get(ref)
        if c is None:
            raise ValueError('%s 引用了不存在的原理：%s' % (item['id'], ref))
        groups.append((c['title'], list(c['points'])))
    extras = item.get('principle_extra', [])
    if extras:
        if groups:
            groups[-1][1].extend(extras)
        else:
            groups.append(('本题的具体处理', list(extras)))
    return groups


# ---------------- 匹配 ----------------
def norm(s):
    s = unicodedata.normalize('NFKC', str(s))
    s = re.sub(r'(实验|第\s*\d+\s*次|题目|\s|[，。、：:（）()《》"“”])', '', s)
    return s.lower()


def bigrams(s):
    return {s[i:i + 2] for i in range(len(s) - 1)} or {s}


def score(title, item):
    n = norm(title)
    bn = bigrams(n)
    if n in {norm(a) for a in item.get('aliases', [])}:
        return 2.0
    hit = bn & bigrams(norm(item['title']))
    return len(hit) / max(len(bn), 1)


def ask(prompt, default='1'):
    try:
        return input(prompt).strip() or default
    except EOFError:
        print('（当前无交互输入，按默认 %s 处理）' % default)
        return default


def choose(items, title, assume_yes=False):
    if not title:
        print('本次收录题目：')
        for i, it in enumerate(items, 1):
            print('  %d  %s' % (i, it['title']))
        sel = ask('请选择编号（回车=1）：')
        return items[int(sel) - 1]
    ranked = sorted(((score(title, it), it) for it in items), key=lambda x: -x[0])
    best_score, best = ranked[0]
    if best_score >= 0.75:
        return best
    if best_score >= 0.4 and assume_yes:
        print('（相似度 %.2f，按最接近的一条处理，可加 --pick 指定）' % best_score)
        return best
    print('题目不够明确，候选如下：')
    for i, (s, it) in enumerate(ranked, 1):
        print('  %d  %s（相似度 %.2f）' % (i, it['title'], s))
    sel = ask('请选择编号（回车=1，0=取消）：')
    if sel == '0':
        raise SystemExit('已取消')
    return ranked[int(sel) - 1][1]


# ---------------- 跑代码 ----------------
def find_gcc():
    """在别的电脑上可能没装 gcc：先找 PATH，再找常见安装位置"""
    for name in (os.environ.get('LABGEN_GCC'), GCC, 'cc', 'clang'):
        if not name:
            continue
        if os.path.isabs(name):
            if os.path.isfile(name):
                return name
            continue
        path = shutil.which(name)
        if path:
            return path
    for p in (os.path.join(BASE, 'gcc', 'bin', 'gcc.exe'),
              os.path.join(BASE, 'mingw64', 'bin', 'gcc.exe'),
              'C:/MinGW/bin/gcc.exe', 'C:/msys64/mingw64/bin/gcc.exe',
              'C:/msys64/ucrt64/bin/gcc.exe', 'C:/TDM-GCC-64/bin/gcc.exe',
              'C:/Program Files/mingw-w64/bin/gcc.exe'):
        if os.path.isfile(p):
            return p
    return None


def run_code(item):
    src = os.path.join(CODE, item['code_file'])
    gcc = find_gcc()
    if not gcc:
        return False, '未找到 C 编译器（gcc）'
    tmp = tempfile.mkdtemp(prefix='labrun_')
    exe = os.path.join(tmp, 'a.exe')
    try:
        c = subprocess.run([gcc, '-std=c99', '-O0', '-o', exe, src],
                           capture_output=True, text=True, encoding='utf-8', errors='replace')
    except OSError as e:
        return False, '无法启动编译器：%s' % e
    if c.returncode != 0:
        return False, '编译失败：\n' + (c.stderr or '')[:800]
    try:
        r = subprocess.run([exe], input=item.get('stdin', ''), capture_output=True,
                           text=True, encoding='utf-8', errors='replace', timeout=30)
    except (OSError, subprocess.TimeoutExpired) as e:
        return False, '运行失败：%s' % e
    if r.returncode != 0:
        return False, '运行异常（退出码 %d）：\n%s' % (r.returncode, (r.stderr or '')[:800])
    return True, r.stdout.replace('\r\n', '\n').strip()


def run_or_fallback(item):
    """编译运行；没有 gcc 时退回到题库里已核验过的样例输出"""
    ok, output = run_code(item)
    if ok:
        return output, 'run'
    expect = item.get('assert_out', '').strip()
    if expect:
        return expect, 'fallback'
    return None, 'fail'


def out_matches(actual, expect):
    return re.sub(r'\s+', ' ', actual.strip()) == re.sub(r'\s+', ' ', expect.strip())


def kb_score(query, concept):
    """按关键词重合度给知识点打分"""
    nq = norm(query)
    ctitle = norm(concept['title'])
    if ctitle and ctitle in nq:
        return 2.0
    bn = bigrams(nq)
    text = concept['title'] + ' ' + ' '.join(concept.get('points', []))
    hit = bn & bigrams(norm(text))
    return len(hit) / max(len(bn), 1)


def kb_options(query, topic=None, subject=None, limit=0):
    """返回 [(相似度, 知识点)]；选定主题时只在这个主题的原理库里排序"""
    t = find_topic(topic, subject) if topic else None
    if t:
        concepts = [dict(c, topic=t['name']) for c in t['concepts']]
    else:
        concepts = list(load_kb().values())
    vals = [(kb_score(query, c), c) for c in concepts]
    vals.sort(key=lambda x: -x[0])
    return vals[:limit] if limit else vals


# ---------------- docx ----------------
def shade(p, color):
    pPr = p._p.get_or_add_pPr()
    el = OxmlElement('w:shd')
    el.set(qn('w:val'), 'clear')
    el.set(qn('w:fill'), color)
    pPr.append(el)


def para(doc, text, size=12, bold=False, font='宋体', before=0, after=0,
         line=1.5, indent=0, align=None, color=None):
    p = doc.add_paragraph()
    pf = p.paragraph_format
    pf.space_before = Pt(before)
    pf.space_after = Pt(after)
    pf.line_spacing = line
    if indent:
        pf.first_line_indent = Pt(indent)
    if align is not None:
        p.alignment = align
    r = p.add_run(text)
    r.font.name = font
    r.font.size = Pt(size)
    r.bold = bold
    r._element.rPr.rFonts.set(qn('w:eastAsia'), font)
    if color:
        r.font.color.rgb = RGBColor(*color)
    return p


def block(doc, lines, font='Consolas', size=9.5, fill='F2F2F2'):
    for ln in lines:
        p = doc.add_paragraph()
        pf = p.paragraph_format
        pf.space_before = Pt(0)
        pf.space_after = Pt(0)
        pf.line_spacing = 1.0
        r = p.add_run(ln if ln else ' ')
        r.font.name = font
        r.font.size = Pt(size)
        r._element.rPr.rFonts.set(qn('w:eastAsia'), font)
        if fill:
            shade(p, fill)


def cn_num(n):
    cn = '零一二三四五六七八九十'
    return cn[n] if 0 <= n <= 10 else str(n)


def fill_template(doc, mapping):
    """在封面段落与信息表中替换 {占位符}，尽量保留原有格式"""
    def repl(p):
        for k, v in mapping.items():
            if k in p.text:
                if any(k in r.text for r in p.runs):
                    for r in p.runs:
                        if k in r.text:
                            r.text = r.text.replace(k, v)
                elif p.runs:
                    full = p.text.replace(k, v)
                    p.runs[0].text = full
                    for r in p.runs[1:]:
                        r._element.getparent().remove(r._element)

    for p in doc.paragraphs:
        repl(p)
    for t in doc.tables:
        for row in t.rows:
            for cell in row.cells:
                for p in cell.paragraphs:
                    repl(p)


def render(entries, meta, out_path=None):
    """entries: [(item, 运行输出, 原理分组)]；多题时依次排成实验题1、实验题2"""
    doc = Document(TEMPLATE)
    fill_template(doc, {
        '{subject}': meta['subject'],
        '{project}': entries[0][0]['title'],
        '{session}': meta['session_name'],
        '{major}': meta['major'],
        '{cls}': meta['cls'],
        '{name}': meta['name'],
        '{sid}': meta['sid'],
        '{hours}': meta['hours'],
        '{teacher}': meta['teacher'],
        '{date}': meta['date'],
    })
    def body(text):
        para(doc, text, size=11, font='宋体', before=0, after=0, line=1.0)

    # 一级标题：参考报告里"一、二"为 11pt，"三"为 14pt 仿宋
    def sec(text, before=14):
        para(doc, text, size=11, font='宋体', before=before, after=0, line=1.0)

    def sec14(text, before=14):
        para(doc, text, size=14, font='仿宋_GB2312', before=before, after=0, line=1.0)

    def sub(text, font='宋体'):
        para(doc, text, size=11, font=font, before=3, after=0, line=1.0)

    def blank():
        para(doc, '', size=11, font='宋体', before=0, after=0, line=1.0)

    sec('一、实验目的', before=6)
    titles = '、'.join(e[0]['title'] for e in entries)
    body('本实验以%s为主题，通过编程实践，掌握%s的实现方法，达到以下目的：' % (
        meta['session_name'], titles))
    for i, t in enumerate(entries[0][0].get('objectives', []), 1):
        body('%d、%s' % (i, t))

    merged = []
    seen_title = set()
    for it, _out, principles in entries:
        for title, pts in principles:
            if title in seen_title:
                continue
            seen_title.add(title)
            merged.append((title, pts))

    sec('二、实验原理')
    for i, (title, pts) in enumerate(merged, 1):
        sub('%d、%s' % (i, title))
        for pt in pts:
            body('· ' + pt)

    sec14('三、实验步骤、数据记录及处理')
    for i, (item, output, _principles) in enumerate(entries, 1):
        kind = item.get('kind') or ('验证性' if i == 1 else '设计性')
        if i > 1:
            blank()
        sub('%d、实验题%d（%s实验）' % (i, i, kind), font='仿宋_GB2312')
        blank()
        body('题目：' + item['title'])
        body('目的：' + item.get('aim', ''))
        body('内容：' + item['stem'])

        para(doc, '实现代码：', size=14, font='仿宋_GB2312', before=6, after=0, line=1.0)
        with open(os.path.join(CODE, item['code_file']), encoding='utf-8') as f:
            block(doc, f.read().rstrip('\n').split('\n'), font='仿宋_GB2312', size=10, fill=None)

        para(doc, '运行结果：', size=14, font='仿宋_GB2312', before=6, after=0, line=1.0, indent=20)
        block(doc, output.split('\n'), font='宋体', size=11, fill=None)

    if not out_path:
        os.makedirs(OUT, exist_ok=True)
        fn = '第%s次实验报告  %s%s%s.docx' % (cn_num(meta['no']), meta['cls'],
                                             meta['name'], meta['sid'])
        out_path = os.path.join(OUT, fn)
    doc.save(out_path)
    return out_path


# ---------------- 预览 ----------------
def _para_style(p):
    """取段落的首个有效 run 的字体、字号、加粗与对齐，判断是否代码行"""
    font, size, bold = '', None, False
    for r in p.runs:
        if not r.text.strip():
            continue
        font = r.font.name or ''
        size = r.font.size.pt if r.font.size else None
        bold = bool(r.bold)
        break
    name = font.split(';')[0].strip().lower()
    code = name in ('consolas', 'courier new') or (
        name.startswith('仿宋') and (size or 0) <= 10.5)
    align = {1: 'center', 2: 'right'}.get(int(p.alignment) if p.alignment is not None else 0, 'left')
    return dict(size=size, bold=bold, align=align, code=code)


def doc_blocks(path):
    """把 docx 按文档顺序拆成块，供网页面板与桌面程序预览"""
    doc = Document(path)
    blocks = []
    for child in doc.element.body.iterchildren():
        if child.tag == qn('w:p'):
            p = Paragraph(child, doc)
            text = p.text.rstrip()
            if not text.strip():
                if blocks and blocks[-1]['kind'] != 'gap':
                    blocks.append({'kind': 'gap'})
                continue
            blocks.append(dict(kind='p', text=text, **_para_style(p)))
        elif child.tag == qn('w:tbl'):
            t = Table(child, doc)
            blocks.append({'kind': 'table',
                           'rows': [[c.text.strip() for c in row.cells] for row in t.rows]})
    return blocks


# ---------------- 命令 ----------------
def cmd_list(args):
    subjects = [args.subject] if args.subject else os.listdir(BANK)
    for s in subjects:
        s = resolve_subject(s) if os.path.isdir(os.path.join(BANK, s)) else s
        print('【%s】' % s)
        for t in bank_topics(s):
            items = load_topic(s, t)
            print('  %s（%d 题）' % (t, len(items)))
            if args.topic and args.topic == t:
                for it in items:
                    print('     - %s  %s' % (it['id'], it['title']))


def cmd_generate(args):
    subject = resolve_subject(args.subject)
    topic = args.topic
    no = args.no
    items = load_topic(subject, topic)
    if not items:
        raise SystemExit('主题 %s 下还没有题目，可用 gen.py new 新建' % topic)
    if args.all:
        chosen = items
        print('本次 %d 题 → %s' % (len(chosen), '、'.join(it['title'] for it in chosen)))
    else:
        chosen = [choose(items, args.title, assume_yes=args.yes)]
        print('已匹配 → %s《%s》（%s）'
              % (chosen[0]['id'], chosen[0]['title'], chosen[0].get('chapter', '')))

    kb = load_kb()
    entries = []
    for item in chosen:
        principles = resolve_principle(item, kb)
        print('%s 原理引用 → %s（%d 个概念，共 %d 条要点）'
              % (item['id'], ' + '.join(item.get('principle_refs', [])), len(principles),
                 sum(len(g[1]) for g in principles)))
        ok, output = run_code(item)
        if not ok:
            print(output)
            raise SystemExit('%s 代码没跑通，已中止生成' % item['id'])
        expect = item.get('assert_out', '')
        if expect:
            print('%s 样例核对：%s' % (item['id'],
                                    '一致 ✓' if out_matches(output, expect) else '不一致 ✗（实际见下）'))
            if not out_matches(output, expect):
                print(output)
        entries.append((item, output, principles))

    meta = dict(subject=subject, no=no, session_name=topic,
                sid=args.sid, name=args.name, major=args.major,
                cls=args.cls, teacher=args.teacher, hours=args.hours,
                date=args.date)
    path = render(entries, meta)
    print('已生成 →', path)


def cmd_validate(args):
    subjects = [args.subject] if args.subject else os.listdir(BANK)
    bad = 0
    total = 0
    for s in subjects:
        kb = load_kb()
        print('【%s】知识点 %d 条' % (s, len(kb)))
        for t in bank_topics(s):
            seen = {}
            for it in load_topic(s, t):
                total += 1
                try:
                    refs = resolve_principle(it, kb)
                except ValueError as e:
                    bad += 1
                    print('✗ %s' % e)
                    continue
                npts = sum(len(g[1]) for g in refs)
                dup = seen.get(str(refs))
                seen[str(refs)] = it['id']
                if dup:
                    print('⚠ %s 与 %s 的原理完全相同，建议补 principle_extra' % (it['id'], dup))
                ok, out = run_code(it)
                if not ok:
                    bad += 1
                    print('✗ %s 编译/运行失败' % it['id'])
                    continue
                good = out_matches(out, it.get('assert_out', ''))
                if not good:
                    bad += 1
                print('%s %s %s（原理 %d 节 / %d 条）' % ('✓' if good else '✗', it['id'], it['title'], len(refs), npts))
                if not good:
                    print('   期望：%s' % it.get('assert_out'))
                    print('   实际：%s' % out)
    print('\n共 %d 个条目，异常 %d' % (total, bad))
    sys.exit(1 if bad else 0)


def cmd_new(args):
    subject = resolve_subject(args.subject)
    topic = args.topic
    d = os.path.join(BANK, subject, topic)
    os.makedirs(d, exist_ok=True)
    n = len([f for f in os.listdir(d) if f.endswith('.json')]) + 1
    cid = '%s-%s-%02d' % ('ds', 'new', n)
    skel = dict(id=cid, title=args.title, aliases=[],
                chapter='', stem='', objectives=[''], principle_refs=[], principle_extra=[],
                steps=[''], code_file=cid + '.c',
                stdin='', assert_out='', analysis=[''], conclusion='')
    fn = os.path.join(d, '%02d_%s.json' % (n, re.sub(r'[/\\:*?"<>|]', '_', args.title)))
    with open(fn, 'w', encoding='utf-8') as f:
        json.dump(skel, f, ensure_ascii=False, indent=2)
    print('已新建模板 →', fn)
    print('请补齐内容并在 code/ 下添加同名 .c 文件')


def main():
    ap = argparse.ArgumentParser(description='实验报告生成器 demo')
    sub = ap.add_subparsers(dest='cmd', required=True)

    p = sub.add_parser('list')
    p.add_argument('subject', nargs='?')
    p.add_argument('topic', nargs='?')
    p.set_defaults(func=cmd_list)

    p = sub.add_parser('generate')
    p.add_argument('subject')
    p.add_argument('topic')
    p.add_argument('title', nargs='?', default='')
    p.add_argument('--no', type=int, default=1)
    p.add_argument('--sid', default='252226209122')
    p.add_argument('--name', default='李施贝仑')
    p.add_argument('--major', default='计算机科学与技术')
    p.add_argument('--cls', default='计科251')
    p.add_argument('--teacher', default='钟宏宇')
    p.add_argument('--hours', default='2')
    p.add_argument('--date', default='2026年11月6日')
    p.add_argument('--yes', action='store_true')
    p.add_argument('--all', action='store_true', help='一次输出本次实验的全部题目（实验题1、实验题2）')
    p.set_defaults(func=cmd_generate)

    p = sub.add_parser('validate')
    p.add_argument('subject', nargs='?')
    p.set_defaults(func=cmd_validate)

    p = sub.add_parser('new')
    p.add_argument('subject')
    p.add_argument('topic')
    p.add_argument('title')
    p.set_defaults(func=cmd_new)

    args = ap.parse_args()
    args.func(args)


if __name__ == '__main__':
    main()
