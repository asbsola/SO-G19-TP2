#include <drivers/keyboardDriver.h>
#include <drivers/videoDriver.h>

static unsigned long presses = 0;

void keyboard_handler(){
    /*for(int i=0; i<50; i++)
        for(int j=0; j<50; j++)
    	    putPixel(0x00FF00, 50*presses+i, 100+j);
    presses++;*/
}

int key_detected(){
    return 0;
}

int key_pressed(){
    return 0;
}
