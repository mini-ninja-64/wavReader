#include "wavLoader.h"
#include "quickmap.h"
#include <iostream>
#include <string>
#include <ncurses.h>

#define WIDTH 1024
#define HEIGHT 512

int main(int argc, char* argv[])
{
	wavFile * myCoolWav = NULL;
	sample * myCoolSample;
	unsigned char bitmap[WIDTH*HEIGHT*3];

	if(argc > 1){
		myCoolWav = loadWav(argv[1]);
		int x = 0;
		for(int i = 0; x < WIDTH; i+= myCoolWav->NumberOfSamples/WIDTH ){
			myCoolSample = createSample(myCoolWav, i);
			for(int y = 0; y < HEIGHT; y++){
				if ( (((y-(HEIGHT/2.0))>0) && ( ((y-(HEIGHT/2.0))/(HEIGHT/2.0))*32767.0) < myCoolSample->Amplitude[0]) || (((y-(HEIGHT/2.0))<0) && ( ((y-(HEIGHT/2.0))/(HEIGHT/2.0))*32768.0) > myCoolSample->Amplitude[0]) ){ //if y positive and y is less than amplitude
					//waves
					bitmap[3 * (y * WIDTH + x)] = 0x00;
					bitmap[3 * (y * WIDTH + x)+1] = 0x00;
					bitmap[3 * (y * WIDTH + x)+2] = 0xFF;
				}else if (y==(HEIGHT/2.0)){
					//center bar
					bitmap[3 * (y * WIDTH + x)] = 0xFF;
					bitmap[3 * (y * WIDTH + x)+1] = 0x00;
					bitmap[3 * (y * WIDTH + x)+2] = 0x00;
				}else{
					//bg
					bitmap[3 * (y * WIDTH + x)] = 0xFF;
					bitmap[3 * (y * WIDTH + x)+1] = 0xFF;
					bitmap[3 * (y * WIDTH + x)+2] = 0xFF;
				}
				
			}
			x++;
		}

		writeBMP(argv[2], WIDTH, HEIGHT, bitmap);
	}else{
		return -1;
	}

	
return 0;
}
