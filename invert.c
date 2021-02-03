#include "bitmap.h"

void invertLess8(IMAGE* img){
    //TODO: processing
    return;
}
void invert8(IMAGE* img){
    for(RGBQUAD* p=((RGBQUAD*)(img->extra))+(img->bi.clrUsed-1);p>=(RGBQUAD*)(img->extra);--p){
        p->rgbAlpha=~(p->rgbAlpha);
        p->rgbRed=~(p->rgbRed);
        p->rgbGreen=~(p->rgbGreen);
        p->rgbBlue=~(p->rgbBlue);
    }
}
void invert16(IMAGE* img){
    unsigned char r,g,b;
    for(RGB16* p=((RGB16*)(img->data+img->sizData))-1;p>=(RGB16*)img->data;--p)
        *p=~*p;
}
void invert24(IMAGE* img){
    for(RGBTRIPLE* p=((RGBTRIPLE*)(img->data+img->sizData))-1;p>=(RGBTRIPLE*)img->data;--p){
        p->rgbtRed=~(p->rgbtRed);
        p->rgbtGreen=~(p->rgbtGreen);
        p->rgbtBlue=~(p->rgbtBlue);
    }
}
void invert32(IMAGE* img){
    for(RGBQUAD* p=((RGBQUAD*)(img->data+img->sizData))-1;p>=(RGBQUAD*)img->data;--p){
        p->rgbAlpha=~(p->rgbAlpha);
        p->rgbRed=~(p->rgbRed);
        p->rgbGreen=~(p->rgbGreen);
        p->rgbBlue=~(p->rgbBlue);
    }
}
void (*invfunc[])(IMAGE*) = {invertLess8,invert8,invert16,invert24,invert32};

int invert(char* src, char* dst){
    IMAGE img;
    
	if(!readImage(&img,src)) return 0;

    invfunc[img.sizPxl](&img);

    if(!writeImage(&img,dst)) return 0;
    freeImage(&img);
    return 1;
}