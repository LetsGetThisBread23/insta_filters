#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //iterate through all rows
    for (int i = 0; i < height; i++)
    {
        //and all columns
        for (int j = 0; j < width; j++)
        {
            //create variable that's the avg of the red, green, and blue values of the pixel
            int average_rgb = round((image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.0);

            //reassign that pixel's r, g, and b to equal that single average 
            image[i][j].rgbtRed = image[i][j].rgbtBlue = image[i][j].rgbtGreen = average_rgb;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //for all rows
    for (int i = 0; i < height; i++)
    {
        // and all columns
        for (int j = 0; j < width; j++)
        {
            //create variables with number values equal to the result of applying the sepia formulas to a pixel's r, g, and b
            //vallues. 
            int sepia_red = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepia_green = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepia_blue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            //then, if any of the above yielded values above the maximum for an rgb value, set 
            //that variable to a value of 255
            if (sepia_red > 255)
            {
                sepia_red = 255;
            }
            if (sepia_green > 255)
            {
                sepia_green = 255;
            }
            if (sepia_blue > 255)
            {
                sepia_blue = 255;
            }

            //reassign pixel's r, g, and b values to the sepia values stores in the variables
            image[i][j].rgbtRed = sepia_red;
            image[i][j].rgbtGreen = sepia_green;
            image[i][j].rgbtBlue = sepia_blue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //for all rows
    for (int i = 0; i < height; i++)
    {
        //and all columns (stopping halfway through because the reflection is done over the midpoint of a row)
        for (int j = 0; j < (width / 2); j++)
        {
            //create a temp variable of the RGBTRIPLE datatype
            //this variable is an exact copy of the pixel that the loop is currently on
            //as with the switching the colored liquids between glasses in lecture, 
            //assign the rgb value of the pixel the loop is on to the temp
            //then, assign the rgb value of the current pixel's mirror to the current pixel
            //then, assign the rgb value of teh current pixel to the pixels' mirror
            RGBTRIPLE temp_pixel = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp_pixel;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a temporary image of the same dimensions as the real image
    RGBTRIPLE temp_image[height][width];
    
    //over all rows
    for (int i = 0; i < height; i++)
    {
        //and all columns
        for (int j = 0; j < width; j++)
        {
            //initialize some important variables
            //the "sum" variables will serve to store the sum of the r, g, and b values 
            //of a pixel's neighbor (because this blurring algorithm assigns a pixel's rgb value 
            //to be the average of those of its neighbors)
            int sum_blue = 0;
            int sum_green = 0;
            int sum_red = 0;

            //and the counter will serve to count how many neighborxing pixels there are
            float counter = 0;

            //with the four above variables, we'll be able to calculate the average r,g,b of a pixel's neighbors

            //iterating over the pixels that are 1 to the left or right of image[i][j]
            for (int k = -1; k <= 1; k++)
            {
                //iterating over the pixels that are 1 above or below image[i][j]
                for (int m = -1; m <= 1; m++)
                {
                    //these variables store the array position of the neighboring pixel that the k&m loops are on
                    //also note that this procedure includes the pixel itself in the calculation of the average of its neighbors
                    int i_neighbor = i + k;
                    int j_neighbor = j + m;

                    //check if the value assigned the the "neighbor" variables is outside the bounds of the image
                    //(indicating that the image[i][j] is on some sort of corner or edge)
                    if (i_neighbor < 0 || i_neighbor > (height - 1) || j_neighbor < 0 || j_neighbor > (width - 1))
                    {
                        //learned a new command! 
                        //if the neighbor pixel is outside the bounds of the image, 
                        //then skip what's below and go back to the top of the loop to look at a different pixel
                        continue;
                    }

                    //the "sum' values are set to equal whatever the sum has been up until this point,
                    //(0 for the first time through the loop), plus the r, g, or b value of the neighbor pixel
                    //that the loop is on currently
                    sum_red += image[i_neighbor][j_neighbor].rgbtRed;
                    sum_green += image[i_neighbor][j_neighbor].rgbtGreen;
                    sum_blue += image[i_neighbor][j_neighbor].rgbtBlue;

                    //and add one to the counter, appropriately increasing our denominator 
                    //for when we take the average 
                    counter++;
                }

                //now, crucially we're still in the [i][j] loop, we set the rgb values
                //of our temp image to equal the average r, g, and b of the pixel we're on and its neighbors
                temp_image[i][j].rgbtRed = round(sum_red / counter);
                temp_image[i][j].rgbtGreen = round(sum_green / counter);
                temp_image[i][j].rgbtBlue = round(sum_blue / counter);
            }
        }
    }

    //BRINGING IT ALL HOME
    //over all rows
    for (int i = 0; i < height; i++)
    {
        //and all columns
        for (int j = 0; j < width; j++)
        {
            //make the r, g, and b values of the ORIGINAL image
            //equal to those of the corresponding pixels in the temp image, which already
            //has the blurry pixel values
            image[i][j].rgbtRed = temp_image[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp_image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp_image[i][j].rgbtBlue;
        }
    }
    return;
}
