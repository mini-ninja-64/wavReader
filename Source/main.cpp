#include "wavLoader.h"
#include "quickmap.h"
#include <iostream>
#include <string>
#include <ncurses.h>

#define WIDTH 2500
#define HEIGHT 512

int main(int argc, char* argv[])
{
	wavFile * myCoolWav = NULL;
	sample * myCoolSample;
	unsigned char bitmap[WIDTH*HEIGHT*3];

	if(argc > 2){
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

		//init curses
		initscr(); 
		keypad(stdscr, TRUE);
		nonl();
		cbreak();
		noecho();
		bool running = true;
		int w, h;
		int offset = 0;
		int c;
		getmaxyx(stdscr, h, w);
		int xzoom=1;
		int szoom=1;
		int zoom=0;
		while(running){
			erase();
			int s = offset;
			for(int x = offset; x < w+offset; x+=xzoom){
				if (!((s < 0) || (s > myCoolWav->NumberOfSamples-1))){
					myCoolSample = createSample(myCoolWav, s);
					for(int y = 0; y < h; y++){
						//mvwaddch(stdscr, y, 0, ACS_BLOCK);
						if ( ( (((y-(h/2.0))>0) && ( (((y-(h/2.0))/(h/2.0))*32767.0) < myCoolSample->Amplitude[0]) ) ) || (((y-(h/2.0))<0) && ((((y-(h/2.0))/(h/2.0))*32768.0) > myCoolSample->Amplitude[0])) ){
							//printw("w");
							mvwaddch(stdscr, h-y, x-offset, ACS_BLOCK);
						}
						if (y-(h/2.0)==0){
							mvwaddch(stdscr, h-y, x-offset, '-');
						}
					}
					s+=szoom;
				}
			}
			//printw("w: %i, h: %i\n", w, h);
			//mvwaddch(stdscr, 0, 0, offset);
			move(0, 0);
			printw("char: %i, offset: %i", c, offset);
			refresh();
			//offset++;
			c = getch();
			switch(c){
				case 27: //esc
					running = false;
					break;
				case KEY_LEFT: //left
					offset--;
					if(offset < 0)
						offset=0;
					break;
				case KEY_RIGHT: //right
					offset++;
					if(offset > myCoolWav->NumberOfSamples-1)
						offset = myCoolWav->NumberOfSamples-1;
					break;
				case KEY_SLEFT:
					offset-=(w/3/xzoom)*szoom/8;
					break;
				case KEY_SRIGHT:
					offset+=(w/3/xzoom)*szoom/8;
					break;
					case 259: //zoom in
						zoom++;
						break;
					case 258: //zoom out
						zoom--;
						break;
				default:
					break;
			}
			szoom = 1;
			xzoom = 1;
			if (zoom > 0){
				xzoom=zoom;
			}else if (zoom < 0){
				szoom=-zoom;
			}
			getmaxyx(stdscr, h, w);
		}
		endwin();
	}else{
		return -1;
	}

	
return 0;
}
