#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[]) {
    if (argc == 2)
    {

        string key = argv[1];
        if (strlen(key) != 26) {
            printf("Key must contain 26 characters.\n");
            return 1;
        } else {
            int map[26];
            int seen[26];
            for(int i = 0; i < 26; i++) {
                seen[i] = 0;
            }

            for(int i = 0; i < 26; i++) {
                if (!isalpha(key[i])) {
                    printf("Key must contain 26 characters.\n");
                    return 1;
                }
                int c = toupper(key[i]);
                if (seen[c - 65] == 1) {
                    printf("Key must contain 26 characters.\n");
                    return 1;
                } else {
                    map[i] = c - 65;
                    seen[c - 65] = 1;
                }
            }

            string input = get_string("plaintext: ");
            printf("ciphertext: ");
            for (int i = 0, n = strlen(input); i < n; i++) {
                if (input[i] >= 'a' && input[i] <= 'z') {
                    printf("%c", map[input[i] - 97] + 97);
                } else if (input[i] >= 'A' && input[i] <= 'Z') {
                    printf("%c", map[input[i] - 65] + 65);
                } else {
                    printf("%c", input[i]);
                }
            }

            printf("\n");
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}
