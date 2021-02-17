/********************************How to mirror image********************************
 * 1. Find out 2 symmetric pixel, and swap each other only once. (traverse half-way)
 * 2. Add extra bit info for low bit.
 ***********************************************************************************/

#include "bitmap.h"

int mirror(char* src, char* dst){
    IMAGE img;  // Container for Abstracted info and data of BMP file
    
    if(!readImage(&img,src)) return 0;

    void (*swap)(void*,void*);
    void (*swap_low)(void*,void*,int,int);
    switch(img.bi.bitCount){
        // Low bit requires more info (which bits)
        case 1: swap_low=swap1; goto LOWBIT; break;
        case 4: swap_low=swap4; goto LOWBIT; break;
        // Assiging function by color depth.
        case 8: swap=swap8; break;
        case 16: swap=swap16; break;
        case 24: swap=swap24; break;
        case 32: swap=swap32; break;
        default: goto ERROR; break;
    }

    // Pixel Array Traversal by assigned function.
    for (int y=img.bi.height-1; y>=0; --y)
        for (int x=((img.bi.width>>1)-1); x>=0; --x)
            swap(PIXEL(img,x,y),PIXEL(img,(img.bi.width-x-1),y));
    goto TERMINATE;

LOWBIT: // Pixel Array Traversal by assigned function.
    for (int y=img.bi.height-1; y>=0; --y)
        for (int x=((img.bi.width>>1)-1); x>=0; --x)// give column info to figure out which bit 
            swap_low(PIXEL(img,x,y),PIXEL(img,(img.bi.width-x-1),y),x,(img.bi.width-x-1));

TERMINATE:  // Write output before termination.
    if(!writeImage(&img,dst)) goto ERROR;
    freeImage(&img);
    return 1;   // Succeed

ERROR:      // free allocated memory even if failed.
    freeImage(&img);
    return 0;   // failed in some reason.
}