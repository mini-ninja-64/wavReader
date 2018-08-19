#ifndef QUICKMAP_H
#define QUICKMAP_H

#include <iostream>

/*
how to bitmap - simple, cba to write fancy reader lolll ( that why we missing the pallete ), windows bitmap, damn this is a simple bitmap fuk me lol

|---------------|
|    header		|--type of bitmap(2 byte), size of file(4), reserved1 (2), reserved2 (2), offset to image data(4)
|---------------|
|				|
|  info header	|--size of header (4), w (4), h (4), num of colour planes(2), bits per pixel(2), compression(4), ppm h (4), ppm v (4), num of colours (4), num of important colours (4)
|				|
|---------------|
|				|
|   IMAGE DATA	|--24bit bgr
|				|
|---------------|

(yn, x0)-->(y0,xn)
*/


//92x64 24 bit bitmap header
static const char BMP_HEADER[] = {0x42,0x4D,0x36,0x45,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x5C,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x45,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int writeBMP(char const* filename, unsigned int width, unsigned int height, unsigned char * imagePixels);

#endif /* QUICKMAP_H */