#include "bitmap.h"

void invRGB16(void* p){ *((unsigned short*)p)=~*((unsigned short*)p); }
void invTriple(void* p){
    ((RGBTRIPLE*)p)->rgbtRed=~(((RGBTRIPLE*)p)->rgbtRed);
    ((RGBTRIPLE*)p)->rgbtGreen=~(((RGBTRIPLE*)p)->rgbtGreen);
    ((RGBTRIPLE*)p)->rgbtBlue=~(((RGBTRIPLE*)p)->rgbtBlue);
}
void invQuad(void* p){
    ((RGBQUAD*)p)->rgbAlpha=~(((RGBQUAD*)p)->rgbAlpha);
    ((RGBQUAD*)p)->rgbRed=~(((RGBQUAD*)p)->rgbRed);
    ((RGBQUAD*)p)->rgbGreen=~(((RGBQUAD*)p)->rgbGreen);
    ((RGBQUAD*)p)->rgbBlue=~(((RGBQUAD*)p)->rgbBlue);
}

int invert(char* src, char* dst){
    IMAGE img;
    
    if(!readImage(&img,src)) return 0;
    
    void (*inv)(void*);
    switch(img.bi.bitCount){
        case 1: case 4: case 8: 
            for(RGBQUAD* p=((RGBQUAD*)(img.extra))+(img.bi.clrUsed-1);p>=(RGBQUAD*)(img.extra);--p)
                invQuad(p);
        goto TERMINATE; break;
        case 16: inv=invRGB16; break;
        case 24: inv=invTriple; break;
        case 32: inv=invQuad; break;
        default: goto ERROR; break;
    }

    for (int y=img.bi.height-1; y>=0; --y)
        for (int x=img.bi.width-1; x>=0; --x)
            inv(PIXEL(img,x,y));
    
TERMINATE:
    if(!writeImage(&img,dst)) goto ERROR;
    freeImage(&img);
    return 1;

ERROR:
    freeImage(&img);
    return 0;
}