#include <stdio.h>          // std i/o and file i/o
#include <malloc.h>         // dynamic allocation/free heap memory
#include <math.h>           // double ceil(double);
#include "bitmap.h"         // customed library

int readImage(IMAGE* img, char* fileName){
    FILE *fpBMP=NULL;       // file pointer that points target BMP file.

    if(img==NULL){ puts("IMAGE container is NULL!"); goto ERROR; }
    if(!(fpBMP=fopen(fileName, "rb")))  // open as read-binary-file
        { printf("Failed to get read access to %s!",fileName); goto ERROR; }

    if(fread(&(img->bf),sizeof(BITMAPFILEHEADER),1,fpBMP)<1)
        // if it can't read 1 of BITMAPFILEHEADER 
        { puts("FileHeader read Error!"); goto ERROR; }
    if(img->bf.type != BM)  // if file type is not right BMP file.
        { puts("Unacceptable Bitmap format file!"); goto ERROR; }

    if(fread(&(img->bi),sizeof(BITMAPINFOHEADER),1,fpBMP)<1)
        { puts("InfoHeader read Error!"); goto ERROR; }
    if(img->bi.size != BI_SIZE)
        // if BI is not BITMAPINFOHEADER (Detected by header size)
        { puts("Unsupported BMP info header!"); goto ERROR; }
    if(img->bi.planes != 1) // Corrupted or un-processable
        { puts("BMP has more than one of plane!"); goto ERROR; }
    if(img->bi.compression != BI_RGB && img->bi.compression != BI_BITFIELDS)
        // Only supports these two compression option
        // (BI_RGB: Not compressed / BI_BITFILEDS: By bitfield )
        { puts("Cannot interpret compressed BMP file!"); goto ERROR; } 
    
    if(img->bf.offBits>0x36){
        // if Pixel Array Data shows up after 0x36,
        // there must be extra area between bf.offBits and 0x36.
        img->extra=malloc(img->bf.offBits - 0x36); // Container for extra area.
        // will be freed in void freeImage(IMAGE*);
        if(img->bi.bitCount<=8) // in case it has color table
            img->bi.clrUsed = (1<<img->bi.bitCount);
        if(fread(img->extra,img->bf.offBits-0x36,1,fpBMP)<1)
            { puts("Failed to read extra area!"); goto ERROR; }
    }
    else { img->extra=NULL; }   // it does not have extra area.

    img->sizRow = (img->bi.width)*(img->bi.bitCount);
    if(img->bi.bitCount < 8)    // requires round up.
        img->sizRow=(int)ceil(((double)(img->sizRow))/8.0);
    else img->sizRow/=8;

    // sizRow should contain 'padding' (caused from 4-byte alignment)
	img->sizRow += (PIXEL_ALIGN-((img->sizRow)%PIXEL_ALIGN))%PIXEL_ALIGN;
    img->sizData = (img->sizRow) * (img->bi.height); // whole data size (by byte)

    img->data=(unsigned char*)malloc(img->sizData); // Container for Pixel Array
    // will be freed in void freeImage(IMAGE*);

    fseek(fpBMP, img->bf.offBits, SEEK_SET); // redundant (it should be already here anyway.)
    if(fread(img->data,img->sizData,1,fpBMP)<1)
        { puts("Failed to read BMP data!"); goto ERROR; }
    
    fclose(fpBMP);  // close target file. (Every information required is saved in Image img)
	return 1;       // reading succeed.

ERROR:  // If error happened while reading, still make sure close file.
    if(fpBMP!=NULL)
        fclose(fpBMP);
    return 0;       // reading fail.
}

int writeImage(IMAGE* img, char* fileName){
    FILE *fpBMP=NULL;       // file pointer that points target BMP file.
    
    if(!(fpBMP=fopen(fileName, "wb")))  // open as write-binary-file
        { printf("Failed to get write access to %s!",fileName); goto ERROR; }
    fseek(fpBMP, 0, SEEK_SET);  // redundant. (It should start at offset '0' anyway)
    if(fwrite(&(img->bf),sizeof(BITMAPFILEHEADER),1,fpBMP)<1)
        { puts("FileHeader write Error!"); goto ERROR; }
    if(fwrite(&(img->bi),sizeof(BITMAPINFOHEADER),1,fpBMP)<1)
        { puts("InfoHeader write Error!"); goto ERROR; }
    // No filtering required (un-processable format is already filtered.)

    if(img->bf.offBits>0x36)    // make sure keep extra area.
        if(fwrite(img->extra,img->bf.offBits-0x36,1,fpBMP)<1)
            { puts("Failed to write extra area!"); goto ERROR; }

    fseek(fpBMP, img->bf.offBits, SEEK_SET);
    if(fwrite(img->data,img->sizData,1,fpBMP)<1)
        { puts("Failed to write BMP data!"); goto ERROR; }

    fclose(fpBMP);  // close target file.
	return 1;       // writing succeed.

ERROR:  // If error happened while writing, still make sure close file.
    if(fpBMP!=NULL)
        fclose(fpBMP);
    return 0;       // writing failed.
}

void freeImage(IMAGE *img){
    // make sure free all memory allocated by readImage & writeImage
    if(img->extra) free(img->extra);
    if(img->data) free(img->data);
}

void swap1(void* a, void* b, int bitA, int bitB){   // swapping 1bit value.
    unsigned char b_ = GET1(*((unsigned char*)a),bitA);
    unsigned char a_ = GET1(*((unsigned char*)b),bitB);
    SET1((unsigned char*)a,bitA,a_)
    SET1((unsigned char*)b,bitB,b_)
}
void swap4(void* a, void* b, int partA, int partB){ // swapping 4bit value.
    unsigned char b_ = GET4(*((unsigned char*)a),partA);
    unsigned char a_ = GET4(*((unsigned char*)b),partB);
    SET4((RGB4*)a,partA,a_)
    SET4((RGB4*)b,partB,b_)
}
void swap8(void* a, void* b){                   // swapping 1 byte value.
    unsigned char tmp = *((unsigned char*)a);
    *((unsigned char*)a) = *((unsigned char*)b);
    *((unsigned char*)b) = tmp;
}
void swap16(void* a, void* b){                  // swapping 2 byte value.
    unsigned short tmp = *((unsigned short*)a);
    *((unsigned short*)a) = *((unsigned short*)b);
    *((unsigned short*)b) = tmp;
}
void swap24(void* a, void* b){                  // swapping 3 byte value.
    RGBTRIPLE tmp = *((RGBTRIPLE*)a);
    *((RGBTRIPLE*)a) = *((RGBTRIPLE*)b);
    *((RGBTRIPLE*)b) = tmp;
}
void swap32(void* a, void* b){                  // swapping 4 byte value.
    RGBQUAD tmp = *((RGBQUAD*)a);
    *((RGBQUAD*)a) = *((RGBQUAD*)b);
    *((RGBQUAD*)b) = tmp;
}