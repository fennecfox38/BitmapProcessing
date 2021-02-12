#include "bitmap.h"

void grayRGB16565(void* p){
    unsigned short gray = ( ((RGB16_565*)p)->red + ((((RGB16_565*)p)->green)>>1) + ((RGB16_565*)p)->blue ) /3;
    ((RGB16_565*)p)->red=((RGB16_565*)p)->blue=gray;
    ((RGB16_565*)p)->green=gray<<1;
}
void grayRGB16555(void* p){
    unsigned short gray = ( ((RGB16_555*)p)->red + ((RGB16_555*)p)->green + ((RGB16_555*)p)->blue ) /3;
    ((RGB16_555*)p)->red=((RGB16_555*)p)->green=((RGB16_555*)p)->blue=gray;
}
void grayTriple(void *p){
    ((RGBTRIPLE*)p)->rgbtRed=((RGBTRIPLE*)p)->rgbtGreen=((RGBTRIPLE*)p)->rgbtBlue
        = ( (((RGBTRIPLE*)p)->rgbtRed) + (((RGBTRIPLE*)p)->rgbtGreen) + (((RGBTRIPLE*)p)->rgbtBlue) ) / 3;
}
void grayQuad(void *p){
    ((RGBQUAD*)p)->rgbRed=((RGBQUAD*)p)->rgbGreen=((RGBQUAD*)p)->rgbBlue
        = ( (((RGBQUAD*)p)->rgbRed) + (((RGBQUAD*)p)->rgbGreen) + (((RGBQUAD*)p)->rgbBlue) ) / 3;
}

int gray(char* src, char* dst){
    IMAGE img;
    
    if(!readImage(&img,src)) return 0;

    void (*func)(void*);
    switch(img.bi.bitCount){
        case 1: case 4: case 8: 
            for(RGBQUAD* p=((RGBQUAD*)(img.extra))+(img.bi.clrUsed-1);p>=(RGBQUAD*)(img.extra);--p)
                grayQuad(p);
            goto TERMINATE; break;
        case 16: func=(img.bi.compression==0 ? grayRGB16555 : grayRGB16565); break;
        case 24: func=grayTriple; break;
        case 32: func=grayQuad; break;
        default: goto ERROR; break;
    }

    for (int y=img.bi.height-1; y>=0; --y)
        for (int x=img.bi.width-1; x>=0; --x)
            func(PIXEL(img,x,y));

TERMINATE:
    if(!writeImage(&img,dst)) goto ERROR;
    freeImage(&img);
    return 1;

ERROR:
    freeImage(&img);
    return 0;
}