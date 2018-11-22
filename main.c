#include <stdio.h>
#include <stdlib.h>

#include "./bmp/bmp.h"

//#define IMAGE_PATH "../data/lena.bmp"
#define IMAGE_PATH "../data/parrot.bmp"
//#define IMAGE_PATH "../data/tiger.bmp"

#define NEW_IMAGE_PATH "../data/new_image.bmp"

int main() {
    bmpImage *bytes = getBmpBytes(IMAGE_PATH);

    printf("image width: %d; image height: %d\n", bytes->width, bytes->height);
    
    convertRGBtoGrayScale(bytes, AVERAGE);
    
    //singleColorChannel(bytes, RED);

    createBMPImage(bytes->data, bytes->width, bytes->height, NEW_IMAGE_PATH);

    //write_to_file(bytes);

    free(bytes);

    //writeRGB("../source", "../dest");

    return 0;
}
