  
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = get_int("Height: ");
    while (height < 1 || height > 8) {
        height = get_int("Height: ");
    }
    int width = height * 2 + 2;
    int i, j; 
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (j == height || j == height + 1) {
                printf(" ");
            } else if (j >= height - i - 1 && j < height) {
                printf("#");
            } else if (j <= i + height + 2 && j > height + 1) {
                printf("#");
            } else if (j < height) {
                printf(" ");
            }
        }
        printf("\n");
    }
}
