#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

// Function prototypes
bool is_jpeg(BYTE arr[]);

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    BYTE buffer[BLOCK_SIZE];

    if (file == NULL)
    {
        printf("Cannot open file\n");
        return 1;
    }

    FILE *output = NULL;
    int count = -1;
    char filename[8];

    while (!feof(file))
    {
        if (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file) == 0) break;

        bool jpg = is_jpeg(buffer);
        if (jpg)
        {
            count++;
            sprintf(filename, "%03i.jpg", count);
            output = fopen(filename, "w");
        }

        if (output != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, output);
        }
    }

    fclose(output);
    fclose (file);
}

bool is_jpeg(BYTE arr[])
{
    if (arr[0] != 0xff)
    {
        return false;
    }

    if (arr[1] != 0xd8)
    {
        return false;
    }

    if (arr[2] != 0xff)
    {
        return false;
    }

    BYTE high = arr[3] & 0xf0;
    if (high != 0xe0)
    {
        return false;
    }

    return true;
}
