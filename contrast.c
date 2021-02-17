/************************How to emphasize contrast************************
 * 1. Get 'distance from middle value in Range'. (ex. 0x7F for 0x00~0xFF)
 * 2. Amplify distance by multiplying given factor.
 * 3. Recovery: Add up distance to middle value.
 * 4. Check overflow (or underflow): replace to max or min value if needed.
 *************************************************************************/

#include "bitmap.h"
// Macro defined function that multiplies factor to 'distance from medium'
#define DISPERSE(VAL,MAX,FAC) (((MAX)>>1)+(((VAL)-((MAX)>>1))*(FAC)))
// Macro defined function that checks and fixes overflow and underflow.
#define INRANGE(VAL,MAX) (((VAL)>(MAX))?(MAX):(((VAL)<0)?0:(VAL)))

// Below are contrasting logic for each bit size and format.
void contrastRGB16565(void *p){
    short r=DISPERSE(((RGB16_565*)p)->red,0x1F,1.3);
    short g=DISPERSE(((RGB16_565*)p)->green,0x3F,1.4);
    short b=DISPERSE(((RGB16_565*)p)->blue,0x1F,1.3);
    ((RGB16_565*)p)->red = INRANGE(r,0x1F);
    ((RGB16_565*)p)->green = INRANGE(g,0x3F);
    ((RGB16_565*)p)->blue = INRANGE(b,0x1F);
}
void contrastRGB16555(void *p){
    short r=DISPERSE(((RGB16_555*)p)->red,0x1F,1.3);
    short g=DISPERSE(((RGB16_555*)p)->green,0x1F,1.3);
    short b=DISPERSE(((RGB16_555*)p)->blue,0x1F,1.3);
    ((RGB16_555*)p)->red = INRANGE(r,0x1F);
    ((RGB16_555*)p)->green = INRANGE(g,0x1F);
    ((RGB16_555*)p)->blue = INRANGE(b,0x1F);
}
void contrastTriple(void* p){
    short temp=DISPERSE(((RGBTRIPLE*)p)->rgbtRed,0xFF,1.5);
    ((RGBTRIPLE*)p)->rgbtRed=INRANGE(temp,0xFF);
    temp=DISPERSE(((RGBTRIPLE*)p)->rgbtGreen,0xFF,1.5);
    ((RGBTRIPLE*)p)->rgbtGreen=INRANGE(temp,0xFF);
    temp=DISPERSE(((RGBTRIPLE*)p)->rgbtBlue,0xFF,1.5);
    ((RGBTRIPLE*)p)->rgbtBlue=INRANGE(temp,0xFF);
}
void contrastQuad(void *p){
    short temp=DISPERSE(((RGBQUAD*)p)->rgbRed,0xFF,1.5);
    ((RGBQUAD*)p)->rgbRed=INRANGE(temp,0xFF);
    temp=DISPERSE(((RGBQUAD*)p)->rgbGreen,0xFF,1.5);
    ((RGBQUAD*)p)->rgbGreen=INRANGE(temp,0xFF);
    temp=DISPERSE(((RGBQUAD*)p)->rgbBlue,0xFF,1.5);
    ((RGBQUAD*)p)->rgbBlue=INRANGE(temp,0xFF);
}

int contrast(char* src, char* dst){
    IMAGE img;  // Container for Abstracted info and data of BMP file
    
    if(!readImage(&img,src)) return 0;
    void (*func)(void*);
    switch(img.bi.bitCount){
        case 1: case 4: case 8:     // if it has color table, just process color table only.
            for(RGBQUAD* p=((RGBQUAD*)(img.extra))+(img.bi.clrUsed-1);p>=(RGBQUAD*)(img.extra);--p)
                contrastQuad(p);
            goto TERMINATE; break;  // skipping Pixel Array traversal.

        // Assiging function by color depth.
        case 16: func=(img.bi.compression==0 ? contrastRGB16555 : contrastRGB16565); break;
        case 24: func=contrastTriple; break;
        case 32: func=contrastQuad; break;
        default: goto ERROR; break; // ???: Error!
    }

    // Pixel Array Traversal by assigned function.
    for (int y=img.bi.height-1; y>=0; --y)
        for (int x=img.bi.width-1; x>=0; --x)
            func(PIXEL(img,x,y));

TERMINATE:  // Write output before termination.
    if(!writeImage(&img,dst)) goto ERROR;
    freeImage(&img);
    return 1;   // Succeed

ERROR:      // free allocated memory even if failed.
    freeImage(&img);
    return 0;   // failed in some reason.
}