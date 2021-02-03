#include <stdio.h>
#include <malloc.h>
#include "bitmap.h"

int readImage(IMAGE* img, char* fileName){
    FILE *fpBMP;

    if(img==NULL){ puts("IMAGE container is NULL!"); goto ERROR; }
	if(!(fpBMP=fopen(fileName, "rb")))
        { puts("Failed to get read access to BMP file!"); goto ERROR; }

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

    img->sizPxl = img->bi.bitCount/8;        // Bytes per Pixels

    img->padding = ((img->bi.width)*(img->sizPxl))%PIXEL_ALIGN;
	if(img->padding) img->padding = PIXEL_ALIGN-(img->padding);
    
    img->sizData = ((img->bi.width)*(img->sizPxl)+img->padding)*(img->bi.height);
	img->data=(char*)malloc(img->sizData);
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
        { puts("Failed to get write access to BMP file!"); goto ERROR; }
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