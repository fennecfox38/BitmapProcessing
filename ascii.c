#include <stdio.h>
#include "bitmap.h"
char ascii[] = { '#', '#', '@', '%', '=', '+', '*', ':', '-', '.', ' ' };

char* asciiLess8(IMAGE* img, int index){
    //TODO: processing
    return ascii;
}
char* ascii8(IMAGE* img, int index){
    RGBQUAD* pixel = ((RGBQUAD*)(img->extra))+(img->data[index]);
    unsigned short gray = (pixel->rgbRed + pixel->rgbGreen + pixel->rgbBlue) / 3;
    return ascii+(gray * sizeof(ascii) / 256);
}
char* ascii16(IMAGE* img, int index){
    RGB16* pixel = (RGB16*)((img->data)+index);
    unsigned short gray = img->bi.compression
        ? ( R565(*pixel) + G565(*pixel) + B565(*pixel) )
        : ( R555(*pixel) + G555(*pixel) + B555(*pixel) );
    return ascii+(gray * sizeof(ascii) / (img->bi.compression?  128 : 96));
}
char* ascii24(IMAGE* img, int index){
    RGBTRIPLE* pixel = (RGBTRIPLE *)((img->data)+index);
    unsigned short gray = (pixel->rgbtRed + pixel->rgbtGreen + pixel->rgbtBlue) / 3;
    return ascii+(gray * sizeof(ascii) / 256);
}
char* ascii32(IMAGE* img, int index){
    RGBQUAD* pixel = (RGBQUAD*)((img->data)+index);
    unsigned short gray = (pixel->rgbRed + pixel->rgbGreen + pixel->rgbBlue) / 3;
    return ascii+(gray * sizeof(ascii) / 256);
}
char* (*asciifunc[])(IMAGE*,int) = {asciiLess8,ascii8,ascii16,ascii24,ascii32};

int toASCII(char* src, char* dst){
	FILE *fpTXT;
	IMAGE img;
	if(!readImage(&img,src)) return 0;
    
	fpTXT = fopen(dst, "w");    // 결과 출력용 텍스트 파일 열기
    if (fpTXT == NULL){		// 파일 열기에 실패하면
        freeImage(&img);	// 픽셀 데이터를 저장한 동적 메모리 해제
        return 0;			// 프로그램 종료
    }
	// 픽셀 데이터는 아래 위가 뒤집혀서 저장되므로 아래쪽부터 반복
    char* (*ascii)(IMAGE*,int) = asciifunc[img.sizPxl];
    for (int y=img.bi.height-1; y>=0; --y){
        for (int x=0; x<img.bi.width; ++x){
            int index =  img.sizPxl
                ? (y*(img.bi.width*img.sizPxl) + (y*img.padding)) + (x*img.sizPxl)
                : (y*(img.bi.width*img.bi.bitCount)/8 + (y*img.padding)) + (x*img.bi.bitCount)/8;

            // RGBTRIPLE 구조체로 파랑, 초록, 빨강값을 가져옴
            // 파랑, 초록, 빨강값의 평균을 구하면 흑백 이미지를 얻을 수 있음
            
            // 흑백값에 ASCII 문자의 개수를 곱한 뒤 256으로 나누면 흑백값에 따라 
            // ASCII 문자의 인덱스를 얻을 수 있음
            char* c = ascii(&img,index);

            // 비트맵 이미지에서는 픽셀의 가로, 세로 크기가 똑같지만
            // 보통 ASCII 문자는 세로로 길쭉한 형태이므로 정사각형 모양과 비슷하게 보여주기 위해
            // 같은 문자를 두 번 저장해줌
            fprintf(fpTXT, "%c%c", *c, *c);    // 텍스트 파일에 문자 출력
        }

        fprintf(fpTXT, "\n");    // 가로 픽셀 저장이 끝났으면 줄바꿈을 해줌
    }

    fclose(fpTXT);    // 텍스트 파일 닫기

	freeImage(&img);
	return 1;
}