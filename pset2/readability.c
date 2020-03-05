#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    string s = get_string("Text: ");
    int words = 0;
    int sentences = 0;
    int letters = 0;

    for (int i = 0, n = strlen(s); i < n; i++) {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) {
            letters++;
        } else if (s[i] == ' ') {
            words++;
        } else if (s[i] == '!' || s[i] == '.' || s[i] == '?') {
            sentences++;
        }
    }

    // to account for the last word
    words++;

    float L = (float)letters / (float)words * 100;
    float S = (float)sentences / (float)words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    int grade = (int)roundf(index);

    if (grade < 1) {
        printf("Before Grade 1\n");
    } else if (grade > 16) {
        printf("Grade 16+\n");
    } else {
        printf("Grade %i\n", grade);
    }
}
