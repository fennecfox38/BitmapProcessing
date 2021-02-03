#include "bitmap.h"

#define IDX1(IMG,X,Y) (((IMG)->data)+((Y)*((IMG)->bi.width*(IMG)->sizPxl) + ((Y)*(IMG)->padding)) + ((X)*(IMG)->sizPxl))
#define IDX2(IMG,X,Y) (((IMG)->data)+((Y)*((IMG)->bi.width*(IMG)->sizPxl) + ((Y)*(IMG)->padding)) + (((IMG)->bi.width-(X)-1)*(IMG)->sizPxl))

void swapLess8(IMAGE* img, int x, int y){
    //TODO: processing
    //index 1 (y*(img.bi.width*img.bi.bitCount)/8 + (y*img.padding)) + (x*img.bi.bitCount)/8
    //index 2 (y*(img.bi.width*img.bi.bitCount)/8 + (y*img.padding)) + ((img.bi.width-x-1)*img.bi.bitCount)/8)
    return;
}
void swap8(IMAGE* img, int x, int y){
    unsigned char* a = (unsigned char *)IDX1(img,x,y);
    unsigned char* b = (unsigned char *)IDX2(img,x,y);
    unsigned char tmp = *a;
    *a = *b;
    *b = tmp;
}
void swap16(IMAGE* img, int x, int y){
    RGB16* a = (RGB16 *)IDX1(img,x,y);
    RGB16* b = (RGB16 *)IDX2(img,x,y);
    RGB16 tmp = *a;
    *a = *b;
    *b = tmp;
    
}
void swap24(IMAGE* img, int x, int y){
    RGBTRIPLE* a = (RGBTRIPLE *)IDX1(img,x,y);
    RGBTRIPLE* b = (RGBTRIPLE *)IDX2(img,x,y);
    RGBTRIPLE tmp = *a;
    *a = *b;
    *b = tmp;
}
void swap32(IMAGE* img, int x, int y){
    RGBQUAD* a = (RGBQUAD *)IDX1(img,x,y);
    RGBQUAD* b = (RGBQUAD *)IDX2(img,x,y);
    RGBQUAD tmp = *a;
    *a = *b;
    *b = tmp;
}
void (*swapfunc[])(IMAGE*,int,int) = {swapLess8,swap8,swap16,swap24,swap32};

int mirror(char* src, char* dst){
    IMAGE img; 
    
	if(!readImage(&img,src)) return 0;
    void (*swap)(IMAGE*,int,int) = swapfunc[img.sizPxl];

    for (int y=img.bi.height-1; y>=0; --y)
        for (int x=(img.bi.width>>1)-1; x>=0; --x)
            swap(&img,x,y);
        

    if(!writeImage(&img,dst)) return 0;
    freeImage(&img);
    return 1;
}