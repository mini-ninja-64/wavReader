#include "wavLoader.h"
#include <iostream>
#include <string>
#include <ncurses.h>

int main(int argc, char* argv[])
{
	wavFile * myCoolWav;
	sample * myCoolSample;
	if(argc > 1){
		myCoolWav = loadWav(argv[1]);
		myCoolSample = createSample(myCoolWav, 5);
		//std::cout << myCoolSample->Amplitude[0] << std::endl;

	}else{
		return -1;
	}

	
return 0;
}
