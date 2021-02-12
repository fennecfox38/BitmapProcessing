#include "bitmap.h"

int mirror(char* src, char* dst){
    IMAGE img; 
    
    if(!readImage(&img,src)) return 0;

    void (*swap)(void*,void*);
    void (*swap_low)(void*,void*,int,int);
    switch(img.bi.bitCount){
        case 1: swap_low=swap1; goto PALETTE; break;
        case 4: swap_low=swap4; goto PALETTE; break;
        case 8: swap=swap8; break;
        case 16: swap=swap16; break;
        case 24: swap=swap24; break;
        case 32: swap=swap32; break;
        default: goto ERROR; break;
    }

    for (int y=img.bi.height-1; y>=0; --y)
        for (int x=((img.bi.width>>1)-1); x>=0; --x)
            swap(PIXEL(img,x,y),PIXEL(img,(img.bi.width-x-1),y));
    goto TERMINATE;

PALETTE:
    for (int y=img.bi.height-1; y>=0; --y)
        for (int x=((img.bi.width>>1)-1); x>=0; --x)
            swap_low(PIXEL(img,x,y),PIXEL(img,(img.bi.width-x-1),y),x,(img.bi.width-x-1));

TERMINATE:
    if(!writeImage(&img,dst)) goto ERROR;
    freeImage(&img);
    return 1;

ERROR:
    freeImage(&img);
    return 0;
}