//
// Created by Munawar Hasan on 14/11/18.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

uint32_t getBMPSize(char *path){
    uint32_t bytes = 0;
    FILE *fp = NULL;
    fp = fopen(path, "r");
    if (fp == NULL)
        return ERROR;
    else{
        uint8_t data[6] = {0};
        fread(data, 6, 1, fp);
        if (data[0] != 0x42 && data[1] != 0x4d)
            return ERROR;
        else
            bytes = data[2] | data[3] << 8 | data[4] << 16 | data[5] << 24;
        fclose(fp);
    }
    return bytes;
}

void readLittleEndian(void *destination, const uint8_t size, FILE *fp){
    switch (size){
        case 2:
            {
                uint8_t data[2] = {0};
                fread(data, 2, 1, fp);
                *((uint16_t *)destination) = (uint16_t) (data[0] | data[1] << 8);
                break;
            }
        case 4:
            {
                uint8_t data[4] = {0};
                fread(data, 4, 1, fp);
                *((uint32_t *)destination) = (uint32_t) (data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24);
                break;
            }
        default:
            printf(".....\n");
    }
}

void getBMPHeader(BMPHeader *bmpHeader, FILE *fp){

    fread(&(bmpHeader->magicNumber), 2, 1, fp);

    if (bmpHeader->magicNumber[0] == 0x42 && bmpHeader->magicNumber[1] == 0x4d){

        readLittleEndian(&(bmpHeader->fileSize), 4, fp);
        readLittleEndian(&(bmpHeader->unusedByte1), 2, fp);
        readLittleEndian(&(bmpHeader->unusedByte2), 2, fp);
        readLittleEndian(&(bmpHeader->pixel_offset), 4, fp);

        //TBD: more than 40 bytes

        //dib header
        readLittleEndian(&(bmpHeader->dibHeader.dibHeaderSize), 4, fp);
        readLittleEndian(&(bmpHeader->dibHeader.width), 4, fp);
        readLittleEndian(&(bmpHeader->dibHeader.height), 4, fp);

        readLittleEndian(&(bmpHeader->dibHeader.colorPlanes), 2, fp);
        readLittleEndian(&(bmpHeader->dibHeader.bits_per_pixel), 2, fp);
        readLittleEndian(&(bmpHeader->dibHeader.bitCompression), 4, fp);

        readLittleEndian(&(bmpHeader->dibHeader.size), 4, fp);
        readLittleEndian(&(bmpHeader->dibHeader.printX), 4, fp);
        readLittleEndian(&(bmpHeader->dibHeader.printY), 4, fp);
        readLittleEndian(&(bmpHeader->dibHeader.colors_in_palette), 4, fp);
        readLittleEndian(&(bmpHeader->dibHeader.imp_colors), 4, fp);

    } else{
        perror("not a bmp file\n");
    }

}


bmpImage *getBmpBytes(char *path){

    FILE *fp = NULL;

    fp = fopen(path, "r");

    if (fp == NULL){
        perror("unable to open file\n");
    } else{
        BMPHeader *bmpHeader = (BMPHeader *)malloc(sizeof(BMPHeader));

        getBMPHeader(bmpHeader, fp);

        uint8_t *data = (uint8_t *)calloc(bmpHeader->dibHeader.width * bmpHeader->dibHeader.height * 3, 1);

        /*for (int i = 0; i < (bmpHeader->dibHeader.width * bmpHeader->dibHeader.height * 3); ++i) {
            fread(&(data[i]), 1, 1, fp);
        }*/

        fread(&(data[0]), (bmpHeader->dibHeader.width * bmpHeader->dibHeader.height * 3), 1, fp);

        imageBytes *bytes = (imageBytes *)calloc((bmpHeader->dibHeader.width * bmpHeader->dibHeader.height), sizeof(imageBytes));

        int count = 0;
        for (int i = 0; i < (bmpHeader->dibHeader.width * bmpHeader->dibHeader.height); ++i) {
            int j = 3 * i;
            uint8_t r = data[j++];
            uint8_t g = data[j++];
            uint8_t b = data[j];

            bytes[count].r = r;
            bytes[count].g = g;
            bytes[count].b = b;
            //bytes[count].alpha = data[i];
            count++;
        }

        fclose(fp);

        bmpImage *image = (bmpImage *) malloc(sizeof(bmpImage));
        image->width = bmpHeader->dibHeader.width;
        image->height = bmpHeader->dibHeader.height;

        image->data = (imageBytes *)calloc(image->height * image->width, sizeof(imageBytes));

        for (int i = 0; i < image->height; ++i) {
            for (int j = 0; j < image->width; j++){
                image->data[(image->height - i - 1) * image->width + j].r = bytes[i * image->width + j].r;
                image->data[(image->height - i - 1) * image->width + j].g = bytes[i * image->width + j].g;
                image->data[(image->height - i - 1) * image->width + j].b = bytes[i * image->width + j].b;
            }
        }

        free(bytes);
        free(bmpHeader);

        return image;
    }

    return NULL;
}

void write_r(bmpImage *image, char *path){
    uint8_t *r = (uint8_t *)malloc(sizeof(uint8_t) * image->width*image->height);
    for (int i = 0; i < (image->width*image->height); i++)
        r[i] = image->data[i].r;
    FILE *fp = NULL;
    fp = fopen(path, "w");
    fwrite(r, 1, (image->width*image->height), fp);
    fclose(fp);
}

void write_g(bmpImage *image, char *path){
    uint8_t *g = (uint8_t *)malloc(sizeof(uint8_t) * image->width*image->height);
    for (int i = 0; i < (image->width*image->height); i++)
        g[i] = image->data[i].g;
    FILE *fp = NULL;
    fp = fopen(path, "w");
    fwrite(g, 1, (image->width*image->height), fp);
    fclose(fp);
}

void write_b(bmpImage *image, char *path){
    uint8_t *b = (uint8_t *)malloc(sizeof(uint8_t) * image->width*image->height);
    for (int i = 0; i < (image->width*image->height); i++)
        b[i] = image->data[i].b;
    FILE *fp = NULL;
    fp = fopen(path, "w");
    fwrite(b, 1, (image->width*image->height), fp);
    fclose(fp);
}

void write_to_file(bmpImage *image){
    //default is r
    uint8_t *r = (uint8_t *)malloc(sizeof(uint8_t) * image->width*image->height);
    for (int i = 0; i < (image->width*image->height); i++)
        r[i] = image->data[i].r;
    FILE *fp = NULL;
    fp = fopen("../data/1data", "w");
    fwrite(r, 1, (image->width*image->height), fp);
    fclose(fp);
}

void toByteArrayLittleEndian(void *input, uint8_t *destination, uint8_t size){
    switch (size){
        case 2:
            {
                destination[0] = (uint8_t) (*((uint16_t *) input));
                destination[1] = (uint8_t) (*((uint16_t *) input) >> 8);
                break;
            }
        case 4:
            {
                destination[0] = (uint8_t) (*((uint32_t *) input));
                destination[1] = (uint8_t) (*((uint32_t *) input) >> 8);
                destination[2] = (uint8_t) (*((uint32_t *) input) >> 16);
                destination[3] = (uint8_t) (*((uint32_t *) input) >> 24);
                break;
            }
        default:
            printf(".....\n");
    }
}

uint8_t paddingBytes(uint32_t width){
    return (uint8_t)((width * 3) % 4);
}

void createBMPHeader(uint8_t *bmpData, uint32_t fileSize, uint32_t height, uint32_t width){

    //magic number
    bmpData[0] = 0x42;
    bmpData[1] = 0x4d;

    //size
    toByteArrayLittleEndian(&fileSize, &bmpData[2], 4);

    //reserved bytes
    bmpData[6] = bmpData[7] = bmpData[8] = bmpData[9] = 0x00;

    uint32_t offSet = 14 + 40;

    //pixel offset
    toByteArrayLittleEndian(&offSet, &bmpData[10], 4);

    //DIB HEADER

    uint32_t dibHeaderSize = 40;
    toByteArrayLittleEndian(&dibHeaderSize, &bmpData[14], 4);

    //width of image
    toByteArrayLittleEndian(&width, &bmpData[18], 4);

    //height of image
    toByteArrayLittleEndian(&height, &bmpData[22], 4);

    //color planes
    uint16_t colorPlanes = 1;
    toByteArrayLittleEndian(&colorPlanes, &bmpData[26], 2);

    //bits in colors
    uint16_t bits_in_colors = 24;
    toByteArrayLittleEndian(&bits_in_colors, &bmpData[28], 2);

    //compression
    uint32_t compressionUsed = 0;
    toByteArrayLittleEndian(&compressionUsed, &bmpData[30], 4);

    //raw size
    uint32_t rawSize = width * height * 3;
    toByteArrayLittleEndian(&rawSize, &bmpData[34], 4);

    //zeroize
    uint32_t zeros = 0;
    toByteArrayLittleEndian(&zeros, &bmpData[38], 4);
    toByteArrayLittleEndian(&zeros, &bmpData[42], 4);
    toByteArrayLittleEndian(&zeros, &bmpData[48], 4);
    toByteArrayLittleEndian(&zeros, &bmpData[52], 4);

}

void createBMPImage(imageBytes *bytes, uint32_t width, uint32_t height, char *path){

    uint8_t padding = paddingBytes(width);

    uint32_t fileSize = 14 + 40 + (width * height * 3) + (height * padding);

    uint8_t *bmpData = (uint8_t *)calloc(fileSize, sizeof(uint8_t));
    createBMPHeader(bmpData, fileSize, height, width);

    //write data to buffer
    int offSet = 54;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            bmpData[offSet + ((height - i - 1) * width * 3) + (3*j)] = bytes[i * width + j].r;
            bmpData[offSet + ((height - i - 1) * width * 3) + (3*j + 1)] = bytes[i * width + j].g;
            bmpData[offSet + ((height - i - 1) * width * 3) + (3*j + 2)] = bytes[i * width + j].b;
        }
    }

    FILE *fp = NULL;
    fp = fopen(path, "w");
    fwrite(bmpData, 1, fileSize, fp);
    fclose(fp);

    printf("image created successfully at path: %s\n", path);

    free(bmpData);
}

void writeRGB(char *sourceDirectory, char *destinationDirectory){
    if (strcmp(sourceDirectory, destinationDirectory) == 0) {
        printf("source and destination directory are same ....\n");
        exit(0);
    }



}

