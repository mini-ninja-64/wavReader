#include "wavLoader.h"
#include <iostream>
#include <string>
#include <ncurses.h>

int main(int argc, char* argv[])
{
    wavFile * myCoolWav;
    if(argc > 1){
        myCoolWav = loadWav(argv[1]);
    }else{
        return -1;
    }

    
return 0;
}
