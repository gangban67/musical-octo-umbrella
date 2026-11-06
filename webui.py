# -*- coding: utf-8 -*-
"""本地网页测试面板：在浏览器里选主题、题目、原理并生成 docx

用法：py webui.py
然后在 Trae 右侧浏览器面板打开终端里打印的地址（默认 http://127.0.0.1:8765/）
"""
import html
import json
import os
import socket
import traceback
from http.server import BaseHTTPRequestHandler, HTTPServer
from socketserver import ThreadingMixIn
from urllib.parse import urlparse, parse_qs, quote

import gen

HERE = os.path.dirname(os.path.abspath(__file__))
OUTDIR = os.path.join(HERE, 'out', 'web')
MADE = {}  # 本次运行里生成过的文档：只有生成后才能预览或下载


def find_item(bank, item_id):
    for t in gen.bank_topics(bank):
        for it in gen.load_topic(bank, t):
            if it['id'] == item_id:
                return it
    return None


def meta():
    return {
        'subjects': [{'name': s['name'], 'topics': gen.topic_names(s['name'])}
                     for s in gen.load_subjects()],
    }


def do_match(d):
    bank = gen.pick_bank(d.get('subject', ''))
    try:
        no = int(d['no'])
    except (KeyError, TypeError, ValueError):
        raise ValueError('实验次数请填写数字')
    topic = d.get('topic', '')
    query = (d.get('query') or '').strip()
    matched = gen.load_topic(bank, topic)
    pool = matched
    if not pool:
        pool = [it for t in gen.bank_topics(bank) for it in gen.load_topic(bank, t)]
    if query:
        ranked = sorted(((gen.score(query, it), it) for it in pool), key=lambda x: -x[0])
    else:
        ranked = [(0.0, it) for it in pool]
    t = gen.find_topic(topic, d.get('subject', ''))
    concepts = t['concepts'] if t else []
    return {
        'items': [{'id': it['id'], 'title': it['title'], 'score': round(s, 3)} for s, it in ranked],
        'principles': [{'id': c['id'], 'title': c['title'], 'score': 0} for c in concepts],
        'matched': bool(matched),
    }


def do_generate(d):
    if not (d.get('name') or '').strip() or not (d.get('sid') or '').strip():
        raise ValueError('请填写姓名和学号')
    bank = gen.pick_bank(d.get('subject', ''))
    try:
        no = int(d['no'])
    except (KeyError, TypeError, ValueError):
        raise ValueError('实验次数请填写数字')
    topic = d.get('topic', '')
    ids = d.get('items') or ([d.get('item')] if d.get('item') else [])
    items = [it for it in (find_item(bank, i) for i in ids) if it]
    if not items:
        raise ValueError('请先勾选至少一道题目')

    kb = gen.load_kb()
    groups = []
    for pid in d.get('principles') or []:
        c = kb.get(pid)
        if c:
            groups.append((c['title'], list(c['points'])))
    if not groups:
        t = gen.find_topic(topic, d.get('subject', ''))
        groups = [(c['title'], list(c['points'])) for c in t['concepts']] if t else []
    entries = []
    modes = []
    for item in items:
        output, mode = gen.run_or_fallback(item)
        if output is None:
            raise ValueError('《%s》运行失败，且题库里没有可用的样例输出' % item['title'])
        modes.append(mode)
        g = [(t, list(p)) for t, p in groups]
        extras = item.get('principle_extra', [])
        if extras and g:
            g[-1][1].extend(extras)
        entries.append((item, output, g))

    name = gen.cn_num(no)
    fn = '第%s次实验报告  %s%s%s.docx' % (name, d.get('cls', ''), d['name'], d['sid'])
    if d.get('preview'):
        fn = '预览_' + fn
    os.makedirs(OUTDIR, exist_ok=True)
    path = os.path.join(OUTDIR, fn)
    info = dict(subject=(d.get('subject') or bank).strip(), no=no, session_name=topic,
                sid=d['sid'].strip(), name=d['name'].strip(),
                major=(d.get('major') or '计算机科学与技术').strip(),
                cls=d.get('cls', '').strip(), teacher=d.get('teacher', '').strip(),
                hours=(d.get('hours') or '2').strip(), date=(d.get('date') or '').strip())
    gen.render(entries, info, path)
    MADE[fn] = path
    return {'name': fn, 'mode': 'fallback' if 'fallback' in modes else 'run', 'path': path}


def preview_page(name):
    """把生成的 docx 按原字号、对齐排成网页，供直接在面板里预览"""
    blocks = gen.doc_blocks(os.path.join(OUTDIR, name))
    body = []
    for b in blocks:
        if b['kind'] == 'table':
            rows = ''.join(
                '<tr>%s</tr>' % ''.join('<td>%s</td>' % html.escape(c) for c in row)
                for row in b['rows'])
            body.append('<table>%s</table>' % rows)
            continue
        if b['kind'] == 'gap':
            body.append('<div class="gap"></div>')
            continue
        cls = []
        if b['code']:
            cls.append('code')
        if b['align'] in ('center', 'right'):
            cls.append(b['align'])
        style = ''
        if b['size']:
            style += 'font-size:%gpt;' % b['size']
        if b['bold']:
            style += 'font-weight:700;'
        body.append('<p%s%s>%s</p>' % (
            ' class="%s"' % ' '.join(cls) if cls else '',
            ' style="%s"' % style if style else '',
            html.escape(b['text'])))
    return '''<!doctype html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>预览 · %s</title>
<style>
  body{margin:0;background:#eceff3;color:#111;font-family:"SimSun","宋体",serif}
  .bar{position:sticky;top:0;z-index:9;display:flex;gap:14px;align-items:center;
       padding:10px 16px;background:#fff;border-bottom:1px solid #dfe4ea;
       font:13px "Microsoft YaHei",sans-serif;color:#6b7684}
  .bar b{color:#1f2733;font-weight:600}
  .bar a{color:#2f6fed;text-decoration:none}
  .bar a:hover{text-decoration:underline}
  .page{width:794px;min-height:1123px;margin:20px auto 48px;background:#fff;
        padding:72px 80px;box-sizing:border-box;box-shadow:0 2px 12px rgba(0,0,0,.12)}
  p{margin:0;line-height:1.45;white-space:pre-wrap;word-break:break-word}
  p.code{font-family:Consolas,"Courier New",monospace;line-height:1.2}
  .center{text-align:center}
  .right{text-align:right}
  .gap{height:.9em}
  table{border-collapse:collapse;width:100%%;margin:10px 0;font-size:11pt}
  td{border:1px solid #444;padding:4px 8px;text-align:center;white-space:pre-wrap}
  @media (max-width:820px){
    .bar{flex-wrap:wrap;gap:6px 12px;padding:8px 12px}
    .page{width:auto;min-height:0;margin:10px;padding:20px 16px;
          overflow-x:auto;box-shadow:none;border-radius:8px}
    td{padding:3px 5px}
  }
</style>
</head>
<body>
<div class="bar">
  <b>文档预览</b><span>%s</span>
  <a href="/">返回面板</a>
  <a href="/api/file?name=%s">下载 docx</a>
</div>
<div class="page">
%s
</div>
</body>
</html>
''' % (html.escape(name), html.escape(name), quote(name), '\n'.join(body))


class Handler(BaseHTTPRequestHandler):
    server_version = 'LabGenPanel/1.0'

    def log_message(self, fmt, *args):
        pass

    # ---------- 输出 ----------
    def _json(self, obj, code=200):
        data = json.dumps(obj, ensure_ascii=False).encode('utf-8')
        self.send_response(code)
        self.send_header('Content-Type', 'application/json; charset=utf-8')
        self.send_header('Content-Length', str(len(data)))
        self.send_header('Cache-Control', 'no-store')
        self.end_headers()
        self.wfile.write(data)

    def _file(self, path, ctype, download=None):
        with open(path, 'rb') as f:
            data = f.read()
        self.send_response(200)
        self.send_header('Content-Type', ctype)
        self.send_header('Content-Length', str(len(data)))
        if ctype.startswith('text/html'):
            self.send_header('Cache-Control', 'no-store')
        if download:
            self.send_header('Content-Disposition',
                             "attachment; filename*=UTF-8''%s" % quote(download))
        self.end_headers()
        self.wfile.write(data)

    def _page(self, text):
        data = text.encode('utf-8')
        self.send_response(200)
        self.send_header('Content-Type', 'text/html; charset=utf-8')
        self.send_header('Content-Length', str(len(data)))
        self.send_header('Cache-Control', 'no-store')
        self.end_headers()
        self.wfile.write(data)

    def _fail(self, msg):
        self._json({'error': msg}, 400)

    # ---------- 路由 ----------
    def do_GET(self):
        u = urlparse(self.path)
        q = parse_qs(u.query)
        try:
            if u.path in ('/', '/index.html'):
                self._file(os.path.join(HERE, 'webui.html'), 'text/html; charset=utf-8')
            elif u.path == '/api/meta':
                self._json(meta())
            elif u.path == '/preview':
                name = os.path.basename(q.get('name', [''])[0])
                if name not in MADE:
                    return self._fail('请先在面板生成文档，再预览')
                self._page(preview_page(name))
            elif u.path == '/api/file':
                name = os.path.basename(q.get('name', [''])[0])
                if name not in MADE:
                    return self._fail('请先在面板生成文档，再下载')
                path = os.path.join(OUTDIR, name)
                if not name.endswith('.docx') or not os.path.isfile(path):
                    return self._fail('文件不存在：%s' % name)
                self._file(path, 'application/vnd.openxmlformats-officedocument.wordprocessingml.document',
                           download=name)
            else:
                self._fail('未知地址：%s' % u.path)
        except Exception as e:
            traceback.print_exc()
            self._fail(str(e))

    def do_POST(self):
        u = urlparse(self.path)
        try:
            n = int(self.headers.get('Content-Length', 0))
            body = json.loads(self.rfile.read(n).decode('utf-8') or '{}')
            if u.path == '/api/match':
                self._json(do_match(body))
            elif u.path == '/api/generate':
                self._json(do_generate(body))
            else:
                self._fail('未知地址：%s' % u.path)
        except Exception as e:
            traceback.print_exc()
            self._fail(str(e))


class Server(ThreadingMixIn, HTTPServer):
    daemon_threads = True
    allow_reuse_address = True


def find_port(start=8765, tries=30):
    for port in range(start, start + tries):
        s = socket.socket()
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            s.bind(('127.0.0.1', port))
            return port
        except OSError:
            continue
        finally:
            s.close()
    raise SystemExit('8765 起的端口都被占用，请先关闭占用的程序')


def lan_ip():
    """取本机局域网 IP，方便手机连同一 Wi-Fi 直接打开"""
    try:
        for ip in socket.gethostbyname_ex(socket.gethostname())[2]:
            if ip.startswith(('192.', '10.', '172.')):
                return ip
    except OSError:
        pass
    return ''


def main():
    if not os.path.isdir(gen.BANK) or not os.path.isfile(gen.KB_FILE):
        print('缺少题库或原理库：%s / %s' % (gen.BANK, gen.KB_FILE))
        return
    # 云端部署时平台会通过 PORT 环境变量指定端口
    port = int(os.environ.get('PORT') or 0) or find_port()
    srv = Server(('0.0.0.0', port), Handler)
    print('测试面板已启动：http://127.0.0.1:%d/' % port)
    ip = lan_ip()
    if ip:
        print('手机访问（与电脑连同一 Wi-Fi）：http://%s:%d/' % (ip, port))
    print('把上面的地址粘贴到浏览器即可；按 Ctrl+C 停止。')
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        print('\n已停止')


if __name__ == '__main__':
    main()
