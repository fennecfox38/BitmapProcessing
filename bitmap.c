#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "bitmap.h"

int readImage(IMAGE* img, char* fileName){
    FILE *fpBMP=NULL;

    if(img==NULL){ puts("IMAGE container is NULL!"); goto ERROR; }
    if(!(fpBMP=fopen(fileName, "rb")))
        { printf("Failed to get read access to %s!",fileName); goto ERROR; }

    if(fread(&(img->bf),sizeof(BITMAPFILEHEADER),1,fpBMP)<1)
        { puts("FileHeader read Error!"); goto ERROR; }
    if(img->bf.type != BM)
        { puts("Unacceptable Bitmap format file!"); goto ERROR; }

    if(fread(&(img->bi),sizeof(BITMAPINFOHEADER),1,fpBMP)<1)
        { puts("InfoHeader read Error!"); goto ERROR; }
    if(img->bi.size != BI_SIZE)
        { puts("Unsupported BMP info header!"); goto ERROR; }
    if(img->bi.planes != 1)
        { puts("BMP has more than one of plane!"); goto ERROR; }
    if(!(img->bi.compression == BI_RGB || img->bi.compression == BI_BITFIELDS))
        { puts("Cannot interpret compressed BMP file!"); goto ERROR; } 
    
    if(img->bf.offBits>0x36){
        img->extra=malloc(img->bf.offBits - 0x36);
        if(img->bi.bitCount<=8)
            img->bi.clrUsed = (1<<img->bi.bitCount);
        if(fread(img->extra,img->bf.offBits-0x36,1,fpBMP)<1)
            { puts("Failed to read extra area!"); goto ERROR; }
    }
    else { img->extra=NULL; }

    img->sizRow = (img->bi.width)*(img->bi.bitCount);

    if(img->bi.bitCount < 8)
        img->sizRow=(int)ceil(((double)(img->sizRow))/8.0);
    else img->sizRow/=8;

	img->sizRow += (PIXEL_ALIGN-((img->sizRow)%PIXEL_ALIGN))%PIXEL_ALIGN;
    img->sizData = (img->sizRow) * (img->bi.height);

    img->data=(unsigned char*)malloc(img->sizData);
    fseek(fpBMP, img->bf.offBits, SEEK_SET);
    if(fread(img->data,img->sizData,1,fpBMP)<1)
        { puts("Failed to read BMP data!"); goto ERROR; }
    
    fclose(fpBMP);
	return 1;

ERROR:
    if(fpBMP!=NULL)
        fclose(fpBMP);
    return 0;
}

int writeImage(IMAGE* img, char* fileName){
    FILE *fpBMP;
    
    if(!(fpBMP=fopen(fileName, "wb")))
        { printf("Failed to get write access to %s!",fileName); goto ERROR; }
    fseek(fpBMP, 0, SEEK_SET);
    if(fwrite(&(img->bf),sizeof(BITMAPFILEHEADER),1,fpBMP)<1)
        { puts("FileHeader write Error!"); goto ERROR; }
    if(fwrite(&(img->bi),sizeof(BITMAPINFOHEADER),1,fpBMP)<1)
        { puts("InfoHeader write Error!"); goto ERROR; }

    if(img->bf.offBits>0x36)
        if(fwrite(img->extra,img->bf.offBits-0x36,1,fpBMP)<1)
            { puts("Failed to write extra area!"); goto ERROR; }

    fseek(fpBMP, img->bf.offBits, SEEK_SET);
    if(fwrite(img->data,img->sizData,1,fpBMP)<1)
        { puts("Failed to write BMP data!"); goto ERROR; }

    fclose(fpBMP);
	return 1;

ERROR:
    if(fpBMP!=NULL)
        fclose(fpBMP);
    return 0;
}

void freeImage(IMAGE *img){
    if(img->extra) free(img->extra);
    if(img->data) free(img->data);
}

void swap1(void* a, void* b, int bitA, int bitB){
    unsigned char b_ = GET1(*((unsigned char*)a),bitA);
    unsigned char a_ = GET1(*((unsigned char*)b),bitB);
    SET1((unsigned char*)a,bitA,a_)
    SET1((unsigned char*)b,bitB,b_)
}
void swap4(void* a, void* b, int partA, int partB){
    unsigned char b_ = GET4(*((unsigned char*)a),partA);
    unsigned char a_ = GET4(*((unsigned char*)b),partB);
    SET4((RGB4*)a,partA,a_)
    SET4((RGB4*)b,partB,b_)
}
void swap8(void* a, void* b){
    unsigned char tmp = *((unsigned char*)a);
    *((unsigned char*)a) = *((unsigned char*)b);
    *((unsigned char*)b) = tmp;
}
void swap16(void* a, void* b){
    unsigned short tmp = *((unsigned short*)a);
    *((unsigned short*)a) = *((unsigned short*)b);
    *((unsigned short*)b) = tmp;
}
void swap24(void* a, void* b){
    RGBTRIPLE tmp = *((RGBTRIPLE*)a);
    *((RGBTRIPLE*)a) = *((RGBTRIPLE*)b);
    *((RGBTRIPLE*)b) = tmp;
}
void swap32(void* a, void* b){
    RGBQUAD tmp = *((RGBQUAD*)a);
    *((RGBQUAD*)a) = *((RGBQUAD*)b);
    *((RGBQUAD*)b) = tmp;
}