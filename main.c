#include "bitmap.h"

int main(void){
	toASCII("128x128_8.bmp","128x128_8.txt");
	gray("128x128_8.bmp","128x128_8_gray.bmp");
	invert("128x128_8.bmp","128x128_8_invert.bmp");
	//mirror("128x128_16_565.bmp","128x128_16_565_mirror.bmp");
    return 0;
}