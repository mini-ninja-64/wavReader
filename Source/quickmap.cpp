#include "quickmap.h"

//TODO: clean up all the things

int writeBMP(char const* filename, unsigned int width, unsigned int height, unsigned char * imagePixels) {
	unsigned char pixelBuffer[width*height*3];
	unsigned char dimensions[8] = {width&0x000000FF,(width&0x0000FF00)>>8,(width&0x00FF0000)>>16,(width&0xFF000000)>>24,
								height&0x000000FF,(height&0x0000FF00)>>8,(height&0x00FF0000)>>16,(height&0xFF000000)>>24};

	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
			pixelBuffer[3 * (y * width + x)] = imagePixels[3 * (y * width + x)+2];
			pixelBuffer[3 * (y * width + x)+1] = imagePixels[3 * (y * width + x)+1];
			pixelBuffer[3 * (y * width + x)+2] = imagePixels[3 * (y * width + x)];
		}
	}

	FILE *fptr;
	fptr = fopen(filename, "wb");
	fseek ( fptr , 0 , SEEK_SET );
	fwrite(BMP_HEADER, sizeof(unsigned char), 0x36, fptr);
	fwrite(pixelBuffer, sizeof(unsigned char), width*height*3, fptr);
	//write width and height
	fseek ( fptr , 18 , SEEK_SET );
	fwrite(dimensions, sizeof(unsigned char), 8, fptr);
	fclose(fptr);
	return 0;
}