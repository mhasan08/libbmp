//
// Created by Munawar Hasan on 14/11/18.
//

#ifndef BMP_BMP_H
#define BMP_BMP_H

#define ERROR 0
#define ERROR_READINGFILE -1
#define ERROR_NOTBMPFILE -2

#include <stdint.h>

enum COLORCHANNEL{
    BIT8,
    BIT24
};

enum GRAYSCALE_CONVERSION_METHOD{
    AVERAGE,
    LUMINOSITY
};

enum COLOR{
    RED,
    GREEN,
    BLUE
};

typedef struct DIBHeader{
    uint32_t dibHeaderSize;
    uint32_t width;
    uint32_t height;
    uint16_t colorPlanes;
    uint16_t bits_per_pixel;
    uint32_t bitCompression;
    uint32_t size;
    uint32_t printX;
    uint32_t printY;
    uint32_t colors_in_palette;
    uint32_t imp_colors;
}DIBHeader;

typedef struct BMPHeader{
    uint8_t magicNumber[2];
    uint32_t fileSize;
    uint16_t unusedByte1;
    uint16_t unusedByte2;
    uint32_t pixel_offset;
    DIBHeader dibHeader;
}BMPHeader;

typedef struct data{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t alpha;
}imageBytes;

typedef struct image {
    imageBytes *data;
    uint32_t width;
    uint32_t height;
}bmpImage;

uint32_t getBMPSize(char *);
bmpImage *getBmpBytes(char *);
void createBMPImage(imageBytes *, uint32_t, uint32_t, char *);
void write_to_file(bmpImage *);
void write_r(bmpImage *, char *);
void write_g(bmpImage *, char *);
void write_b(bmpImage *, char *);

void writeRGB(char *, char *);

void convertRGBtoGrayScale(bmpImage *, enum GRAYSCALE_CONVERSION_METHOD);
void singleColorChannel(bmpImage *, enum COLOR);

#endif //BMP_BMP_H
