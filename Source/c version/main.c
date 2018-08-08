#include "wavLoader.h"

#include <ncurses.h>

int main(int argc, char* argv[])
{
    struct wavFile * myCoolWav;
    if(argc > 1){
        myCoolWav = loadWav(argv[1]);
    }else{
        return -1;
    }

    
return 0;
}
