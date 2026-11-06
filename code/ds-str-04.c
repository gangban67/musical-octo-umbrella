#include <stdio.h>
#include <string.h>

#define MAX 256

const char tab[] = "ngzqtcobmuhelkpdawxfyivrsj";

void Encrypt(const char *plain, char *cipher)
{
    int i;
    for (i = 0; plain[i] != '\0'; i++) {
        if (plain[i] >= 'a' && plain[i] <= 'z')
            cipher[i] = tab[plain[i] - 'a'];
        else
            cipher[i] = plain[i];
    }
    cipher[i] = '\0';
}

void Decrypt(const char *cipher, char *plain)
{
    int i, j;
    for (i = 0; cipher[i] != '\0'; i++) {
        plain[i] = cipher[i];
        if (cipher[i] >= 'a' && cipher[i] <= 'z') {
            for (j = 0; j < 26; j++) {
                if (tab[j] == cipher[i]) {
                    plain[i] = 'a' + j;
                    break;
                }
            }
        }
    }
    plain[i] = '\0';
}

int main(void)
{
    char line[MAX], cipher[MAX], plain[MAX];

    if (fgets(line, sizeof(line), stdin) == NULL)
        return 0;
    line[strcspn(line, "\r\n")] = '\0';

    Encrypt(line, cipher);
    Decrypt(cipher, plain);

    printf("plain=%s\n", line);
    printf("encrypt=%s\n", cipher);
    printf("decrypt=%s\n", plain);
    return 0;
}
