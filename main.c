#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

// create 1D array
unsigned char * uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char)) ;
}

// subtract 2 images
unsigned char *absolute_subtraction_images(unsigned char *image_background, unsigned char *image_foreground, int width, int height, int channel)
{
    unsigned char *new_2d_array = uc_arrayNew_1d( height * width );

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int sum = 0;

            for(int k = 0; k < channel; k++)
            {
                sum +=abs( image_background[i*width*channel + j*channel + k] - image_foreground[i*width*channel + j*channel + k] );
            }

            //new_2d_array[i*width + j] = sum / 3; // test subtract image

            if(sum/3 < 53)
            {
                new_2d_array[i*width + j] = 0;
            }
            else
            {
                new_2d_array[i*width + j] = 255;
            }
        }
    }

    return new_2d_array;
}

// combine 2 images
unsigned char *combine_images(unsigned char *subtract_image, unsigned char *image_foreground, unsigned char *image_weather_forecast, int width, int heigth, int channel)
{
    for(int i = 0; i < heigth; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(subtract_image[i*width + j] == 255) // if the pixel is white
            {
                for(int k = 0; k < channel; k++)
                {
                    image_weather_forecast[i*width*channel + j*channel + k] = image_foreground[i*width*channel + j*channel + k];
                }
            }
        }
    }

    return image_weather_forecast;
}

int main()
{
    // declare variables
    int width , height , channel;

    char path_img_background[] = "./images/background.jpg";
    char save_path_background[] = "./images/background_New.png";

    char path_img_foreground[] = "./images/foreground.jpg";
    char save_path_foreground[] = "./images/background_New.png";

    char path_img_weather_forcast[] = "./images/weather_forecast-resized.jpg";
    char save_path_weather_forcast[] = "./images/weather_forecast-resized_New.png";


    // read image data
    unsigned char * image_background = stbi_load (path_img_background, &width, &height, &channel, 0) ;
    unsigned char * image_foreground = stbi_load (path_img_foreground, &width, &height, &channel, 0) ;
    unsigned char * image_weather_forecast = stbi_load (path_img_weather_forcast, &width, &height, &channel, 0) ;

    if ( image_background == NULL || image_foreground == NULL )
    {
        printf ("\nError in loading the image \n") ;
        exit(1);
    }

    printf ("Width = %d\nHeight = %d\nChannel = %d\n", width, height, channel);

    // subtract 2 images
    unsigned char *subtract_image = absolute_subtraction_images(image_background, image_foreground, width, height, channel);

    /// name of the image
    //char save_path_subtraction_image[] = "./images/image_test-Subtract.png";
    //char save_path_subtraction_image[] = "./images/image-Subtract.png";
    char save_path_subtraction_image[] = "./images/image-Combination.png";

    // combine 2 images
    combine_images(subtract_image, image_foreground, image_weather_forecast, width, height, channel);

    // save image
    //stbi_write_png(save_path_subtraction_image, width, height, 1, subtract_image, width);
    stbi_write_png(save_path_subtraction_image, width, height, channel, image_weather_forecast, width*channel);

    printf("New image saved to %s\n", save_path_subtraction_image);

}
