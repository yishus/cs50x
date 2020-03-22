#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"

typedef struct
{
    int  blue;
    int  green;
    int  red;
} __attribute__((__packed__))
RGBINT;

BYTE average(int count, BYTE arr[]);
RGBTRIPLE average_pixels(int count, RGBTRIPLE pixels[]);
RGBINT Gx(RGBTRIPLE px[3][3]);
RGBINT Gy(RGBTRIPLE px[3][3]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE arr[3] = {image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue};
            BYTE avg = average(3, arr);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    memcpy(copy, image, sizeof copy);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // center
            int count = 1;
            RGBTRIPLE arr[9] = {copy[i][j]};

            // row above
            if (i - 1 >= 0)
            {

                arr[count] = copy[i-1][j];
                count++;

                if (j - 1 >= 0)
                {
                    arr[count] = copy[i-1][j-1];
                    count++;
                }

                if (j + 1 < width)
                {
                    arr[count] = copy[i-1][j+1];
                    count++;
                }
            }

            // current row
            if (j - 1 >= 0)
            {
                arr[count] = copy[i][j-1];
                count++;
            }

            if (j + 1 < width)
            {
                arr[count] = copy[i][j+1];
                count++;
            }

            // row below
            if (i + 1 < height)
            {
                arr[count] = copy[i+1][j];
                count++;

                if (j - 1 >= 0)
                {
                    arr[count] = copy[i+1][j-1];
                    count++;
                }

                if (j + 1 < width)
                {
                    arr[count] = copy[i+1][j+1];
                    count++;
                }
            }

            image[i][j] = average_pixels(count, arr);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    memcpy(copy, image, sizeof copy);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE black = { .rgbtRed = 0, .rgbtGreen = 0, .rgbtBlue = 0};
            RGBTRIPLE matrix[3][3] = {
                {black, black, black},
                {black, black, black},
                {black, black, black}
            };

            if (i - 1 >= 0)
            {
                matrix[0][0] = j - 1 >= 0 ? copy[i-1][j-1] : black;
                matrix[0][1] = copy[i-1][j];
                matrix[0][2] = j + 1 < width ? copy[i-1][j+1] : black;
            }

            matrix[1][0] = j - 1 >= 0 ? copy[i][j-1] : black;
            matrix[1][1] = copy[i][j];
            matrix[1][2] = j + 1 < width ? copy[i][j+1] : black;

            if (i + 1 < height)
            {
                matrix[2][0] = j - 1 >= 0 ? copy[i+1][j-1] : black;
                matrix[2][1] = copy[i+1][j];
                matrix[2][2] = j + 1 < width ? copy[i+1][j+1] : black;
            }

            RGBINT gx = Gx(matrix);
            RGBINT gy = Gy(matrix);

            double red = sqrt(gx.red * gx.red + gy.red * gy.red);
            double green = sqrt(gx.green * gx.green + gy.green * gy.green);
            double blue = sqrt(gx.blue * gx.blue + gy.blue * gy.blue);

            RGBTRIPLE res = { .rgbtRed = red > 255 ? 255 : round(red), .rgbtGreen = green > 255 ? 255 : round(green), .rgbtBlue = blue > 255 ? 255 : round(blue) };
            image[i][j] = res;
        }
    }
    return;
}

BYTE average(int count, BYTE arr[])
{
    float sum = 0;

    for(int j=0; j<count; j++)
    {
        sum += arr[j];

    }

    return round(sum / count);
}

RGBTRIPLE average_pixels(int count, RGBTRIPLE pixels[])
{
    BYTE red, green, blue;
    BYTE red_arr[count], green_arr[count], blue_arr[count];
    RGBTRIPLE px;

    for(int i = 0; i < count; i++)
    {
        red_arr[i] = pixels[i].rgbtRed;
        green_arr[i] = pixels[i].rgbtGreen;
        blue_arr[i] = pixels[i].rgbtBlue;
    }

    red = average(count, red_arr);
    green = average(count, green_arr);
    blue = average(count, blue_arr);

    px = (RGBTRIPLE) { .rgbtRed = red, .rgbtGreen = green, .rgbtBlue = blue };

    return px;
}


RGBINT Gx(RGBTRIPLE px[3][3])
{
    int matrix[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            r_sum += matrix[i][j] * px[i][j].rgbtRed;
            g_sum += matrix[i][j] * px[i][j].rgbtGreen;
            b_sum += matrix[i][j] * px[i][j].rgbtBlue;
        }
    }

    RGBINT res = { .red = r_sum, .green = g_sum, .blue = b_sum };

    return res;
}

RGBINT Gy(RGBTRIPLE px[3][3])
{
    int matrix[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            r_sum += matrix[i][j] * px[i][j].rgbtRed;
            g_sum += matrix[i][j] * px[i][j].rgbtGreen;
            b_sum += matrix[i][j] * px[i][j].rgbtBlue;
        }
    }

    RGBINT res = { .red = r_sum, .green = g_sum, .blue = b_sum };

    return res;
}
