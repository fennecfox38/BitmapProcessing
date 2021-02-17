/**************************How to convert to ascii**************************
 * 1. Add up values of each color channel.
 * 2. Get average. It is Brightness of pixel.
 * 3. define ascii array by how full it is.
 * 4. print ascii character by brightness.
 ***************************************************************************/
#include <stdio.h>
#include "bitmap.h"

// defined ascii array : lower is darker, higher is brighter.
char ascii[] = { '#', '#', '@', '%', '=', '+', '*', ':', '-', '.', ' ' };

// functions that returns corresponding color in color table
RGBQUAD* getTable1(RGBQUAD* table, unsigned char* p, int x){ return table+GET1(*p,x); }
RGBQUAD* getTable4(RGBQUAD* table, unsigned char* p, int x){ return table+GET4(*p,x); }
RGBQUAD* getTable8(RGBQUAD* table, unsigned char* p, int x){ return table+*p; }

// Below are logic finds out brightness and corresponding ascii character.
// by each bit size and format.
char* ascii16565(void* p){
    unsigned short gray = ( ((RGB16_565*)p)->red + ((((RGB16_565*)p)->green)>>1) + ((RGB16_565*)p)->blue ) /3;
    return ascii+(gray * sizeof(ascii) / 32);
}
char* ascii16555(void* p){
    unsigned short gray = ( ((RGB16_555*)p)->red + ((RGB16_555*)p)->green + ((RGB16_555*)p)->blue ) /3;
    return ascii+(gray * sizeof(ascii) / 32);
}
char* asciiTriple(void* p){
    unsigned short gray = (((RGBTRIPLE*)p)->rgbtRed + ((RGBTRIPLE*)p)->rgbtGreen + ((RGBTRIPLE*)p)->rgbtBlue) / 3;
    return ascii+(gray * sizeof(ascii) / 256);
}
char* asciiQuad(void* p){
    unsigned short gray = (((RGBQUAD*)p)->rgbRed + ((RGBQUAD*)p)->rgbGreen + ((RGBQUAD*)p)->rgbBlue) / 3;
    return ascii+(gray * sizeof(ascii) / 256);
}

int toASCII(char* src, char* dst){
    FILE *fpTXT;
    IMAGE img;
    if(!readImage(&img,src)) return 0;
    
    fpTXT = fopen(dst, "w");
    if (fpTXT == NULL) goto ERROR;

    char* (*ascii)(void*);
    RGBQUAD* (*getTable)(RGBQUAD*,unsigned char*,int);
    switch(img.bi.bitCount){
        // Low bit requires additional process (referring color table)
        case 1: getTable=getTable1; goto PALETTE; break;
        case 4: getTable=getTable4; goto PALETTE; break;
        case 8: getTable=getTable8; goto PALETTE; break;

        // Assiging function by color depth.
        case 16: ascii=(img.bi.compression==0 ? ascii16555 : ascii16565); break;
        case 24: ascii=asciiTriple; break;
        case 32: ascii=asciiQuad; break;
        default: goto ERROR; break;
    }

    // Pixel Array Traversal by assigned function.
    for (int y=img.bi.height-1; y>=0; --y){
        for (int x=0; x<img.bi.width; ++x){
            char* c = ascii(PIXEL(img,x,y));
            fprintf(fpTXT, "%c%c", *c, *c);
            // print twice (Adjustment)
            // because character is narrow vertically.
        }
        fprintf(fpTXT, "\n");
        // new line for end of every single row.
    }
    goto TERMINATE; // skip PALETTE

PALETTE: // for low bit only. in order to refer color table
    for (int y=img.bi.height-1; y>=0; --y){
        for (int x=0; x<img.bi.width; ++x){
            char* c = asciiQuad(getTable((RGBQUAD*)img.extra,(unsigned char*)(PIXEL(img,x,y)),x));
            fprintf(fpTXT, "%c%c", *c, *c);
        }
        fprintf(fpTXT, "\n");
    }

TERMINATE:  // Write output before termination.
    fclose(fpTXT);
    freeImage(&img);
    return 1;   // Succeed

ERROR:      // free allocated memory even if failed.
    if(fpTXT) fclose(fpTXT);
    freeImage(&img);
    return 0;   // failed in some reason.
}