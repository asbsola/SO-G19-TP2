#include <keyboard.h>
#include <videoDriver.h>

void keyboard_handler(){
    for(int i=0; i<50; i++)
        for(int j=0; j<50; j++)
    	    putPixel(0x00FF00, 50+i, 100+j);
}

int key_detected(){
    return 0;
}

int key_pressed(){
    return 0;
}