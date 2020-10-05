#include "helpers.h"
#include <math.h>
#include <string.h>

BYTE clampValue(int input)
{
    if (input > 255)
    {
        input = 255;
    }
    if (input < 0)
    {
        input = 0;
    }
    return input;
}

// basic round function
int roundValue(float input)
{
    return (int)(input < 0 ? (input - 0.5f) : (input + 0.5f));
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            BYTE blue = image[row][col].rgbtBlue;
            BYTE green = image[row][col].rgbtGreen;
            BYTE red = image[row][col].rgbtRed;
            
            float sum = blue + green + red;
            float makeGrey = roundValue(sum / 3);
            BYTE gray = (BYTE)(makeGrey);

            image[row][col].rgbtBlue = gray;
            image[row][col].rgbtGreen = gray;
            image[row][col].rgbtRed = gray;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            BYTE blue = image[row][col].rgbtBlue;
            BYTE green = image[row][col].rgbtGreen;
            BYTE red = image[row][col].rgbtRed;

            float sepiaRed = .393 * red + .769 * green + .189 * blue;
            float sepiaGreen = .349 * red + .686 * green + .168 * blue;
            float sepiaBlue = .272 * red + .534 * green + .131 * blue;

            image[row][col].rgbtBlue = clampValue(roundValue(sepiaBlue));
            image[row][col].rgbtGreen = clampValue(roundValue(sepiaGreen));
            image[row][col].rgbtRed = clampValue(roundValue(sepiaRed));
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++) //for each row
    {
        RGBTRIPLE currentRow[width]; //create new array to store column
        for (int col = 0; col < width; col++) //for each column
        {
            currentRow[width - 1 - col] = image[row][col]; //put the row in backwards
        }

        for (int colAgain = 0; colAgain < width; colAgain++) //for each column
        {
            image[row][colAgain] = currentRow[colAgain];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a temporary image to store new values in
    RGBTRIPLE tempImg[height][width];

    //for each pixel in each row
    for (int row = 0; row < height; row++)
    {
        //for each pixel in each column
        for (int col = 0; col < width; col++)
        {
            //make an array to store 3x3 grid for blurring
            RGBTRIPLE box[9];

            //fill box array with 0s - if you don't do this you can get some weird numbers from accessing some point in memory. At least I think that's what's happening
            for (int cl = 0; cl < 9; cl++)
            {
                memset(box, 0, sizeof(RGBTRIPLE) * 9);
            }

            int avgDiv = 0;

            //fill the box array with neighbouring pixels, catering for edges
            if (row > 0)
            {
                box[1] = image[row - 1][col];
                avgDiv++;

                if (col > 0)
                {
                    box[0] = image[row - 1][col - 1];
                    avgDiv++;
                }

                if (col < width - 1)
                {
                    box[2] = image[row - 1][col + 1];
                    avgDiv++;
                }
            }

            if (row < height - 1)
            {
                box[7] = image[row + 1][col];
                avgDiv++;

                if (col > 0)
                {
                    box[6] = image[row + 1][col - 1];
                    avgDiv++;
                }

                if (col < width - 1)
                {
                    box[8] = image[row + 1][col + 1];
                    avgDiv++;
                }
            }

            if (col > 0)
            {
                box[3] = image[row][col - 1];
                avgDiv++;
            }

            box[4] = image[row][col];
            avgDiv++;

            if (col < width - 1)
            {
                box[5] = image[row][col + 1];
                avgDiv++;
            }

            //Find the average of all all values in box
            float avgBlueFl = 0;
            float avgGreenFl = 0;
            float avgRedFl = 0;
            for (int i = 0; i < 9; i++)
            {
                avgBlueFl += box[i].rgbtBlue;
                avgGreenFl += box[i].rgbtGreen;
                avgRedFl += box[i].rgbtRed;
            }
            avgBlueFl /= avgDiv;
            avgGreenFl /= avgDiv;
            avgRedFl /= avgDiv;

            //round the avg value and convert to whole num
            int avgBlue = roundValue(avgBlueFl);
            int avgGreen = roundValue(avgGreenFl);
            int avgRed = roundValue(avgRedFl);

            //stick the avg value into the temporary image
            tempImg[row][col].rgbtBlue = clampValue(avgBlue);
            tempImg[row][col].rgbtGreen = clampValue(avgGreen);
            tempImg[row][col].rgbtRed = clampValue(avgRed);
        }
    }

    //transfer the temporary image into the main image
    memcpy(image, tempImg, width * height * sizeof(RGBTRIPLE));
}