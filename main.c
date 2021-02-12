#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "bitmap.h"

int isPathValid(char* path,int len){
    for(int i=1;i<len;++i){
        if(path[i]=='/'||path[i]=='\\'){
            path[i]='\0';
            if(access(path,W_OK)==-1)
                if(mkdir(path,0777)==-1)
                    return 0;
            path[i]='/';
        }
    }
    return (access(path,W_OK)==0 ? 1 : (mkdir(path,0777)?0:1));
}

int isFileValid(char* file, int mode){
    int len = strlen(file);
    if(mode&R_OK){
        if(strcmp(file+len-4,".bmp")) return 0;
        return (access(file,R_OK)==0);
    }
    if(mode&W_OK){
        if(strcmp(file+len-4,".bmp") && strcmp(file+len-4,".txt")) return 0;
        for(--len;len>=0&&file[len]!='/'&&file[len]!='\\';--len);
        if(len<0) return 1;
        file[len]='\0';
        int res = isPathValid(file,len);
        file[len]='/';
        return res;
    }
    return (access(file,F_OK)==0);
}

int main(int argc, char* argv[]){
    if(argc!=4) goto INVALID;

    if(!isFileValid(argv[2],R_OK)) goto INVALID;
    if(!isFileValid(argv[3],W_OK)) goto INVALID;

    if(argv[1][0]!='-') goto INVALID;
    switch(argv[1][1]){
        case 'a': case 'A': toASCII(argv[2],argv[3]); break;
        case 'c': case 'C': contrast(argv[2],argv[3]); break;
        case 'g': case 'G': gray(argv[2],argv[3]); break;
        case 'i': case 'I': invert(argv[2],argv[3]); break;
        case 'm': case 'M': mirror(argv[2],argv[3]); break;
        default: goto INVALID; break;
    }

    return 0;
    
INVALID:
    if(argc==2 && argv[1][0]=='-' && argv[1][1]=='e') goto EXAMPLE;
    puts("Invalid argument!");
    puts("for example >./bmputil [option] [src filename (w/ extension)] [dst filename (w/ extension)]");
    puts("option: [ toASCII: -a | contrast: -c | gray: -g | invert: -i | mirror -m ]");
    puts("Make sure the path is valid!");
    return 0;

EXAMPLE:
    if(access("source",R_OK)==-1) return -1;
    if(access("result",W_OK)==-1) if(mkdir("result",0777)==-1) return -1;
    if(access("result/lena",W_OK)==-1) if(mkdir("result/lena",0777)==-1) return -1;
    if(access("result/mandrill",W_OK)==-1) if(mkdir("result/mandrill",0777)==-1) return -1;
    if(access("result/parrots",W_OK)==-1) if(mkdir("result/parrots",0777)==-1) return -1;
    if(access("result/pepper",W_OK)==-1) if(mkdir("result/pepper",0777)==-1) return -1;
    if(access("result/sailboat",W_OK)==-1) if(mkdir("result/sailboat",0777)==-1) return -1;

    toASCII("source/lena/1_mono.bmp","result/lena/1.txt");
    toASCII("source/lena/4.bmp","result/lena/4.txt");
    toASCII("source/lena/8.bmp","result/lena/8.txt");
    toASCII("source/lena/16_555.bmp","result/lena/16_555.txt");
    toASCII("source/lena/16_565.bmp","result/lena/16_565.txt");
    toASCII("source/lena/24.bmp","result/lena/24.txt");
    toASCII("source/lena/32.bmp","result/lena/32.txt");
    contrast("source/lena/4.bmp","result/lena/4_contrast.bmp");
    contrast("source/lena/8.bmp","result/lena/8_contrast.bmp");
    contrast("source/lena/16_555.bmp","result/lena/16_555_contrast.bmp");
    contrast("source/lena/16_565.bmp","result/lena/16_565_contrast.bmp");
    contrast("source/lena/24.bmp","result/lena/24_contrast.bmp");
    contrast("source/lena/32.bmp","result/lena/32_contrast.bmp");
    gray("source/lena/4.bmp","result/lena/4_gray.bmp");
    gray("source/lena/8.bmp","result/lena/8_gray.bmp");
    gray("source/lena/16_555.bmp","result/lena/16_555_gray.bmp");
    gray("source/lena/16_565.bmp","result/lena/16_565_gray.bmp");
    gray("source/lena/24.bmp","result/lena/24_gray.bmp");
    gray("source/lena/32.bmp","result/lena/32_gray.bmp");
    invert("source/lena/1_mono.bmp","result/lena/1_invert.bmp");
    invert("source/lena/4.bmp","result/lena/4_invert.bmp");
    invert("source/lena/8.bmp","result/lena/8_invert.bmp");
    invert("source/lena/16_555.bmp","result/lena/16_555_invert.bmp");
    invert("source/lena/16_565.bmp","result/lena/16_565_invert.bmp");
    invert("source/lena/24.bmp","result/lena/24_invert.bmp");
    invert("source/lena/32.bmp","result/lena/32_invert.bmp");
    mirror("source/lena/1_mono.bmp","result/lena/1_mirror.bmp");
    mirror("source/lena/4.bmp","result/lena/4_mirror.bmp");
    mirror("source/lena/8.bmp","result/lena/8_mirror.bmp");
    mirror("source/lena/16_555.bmp","result/lena/16_555_mirror.bmp");
    mirror("source/lena/16_565.bmp","result/lena/16_565_mirror.bmp");
    mirror("source/lena/24.bmp","result/lena/24_mirror.bmp");
    mirror("source/lena/32.bmp","result/lena/32_mirror.bmp");
    
    toASCII("source/mandrill/1_mono.bmp","result/mandrill/1.txt");
    toASCII("source/mandrill/4.bmp","result/mandrill/4.txt");
    toASCII("source/mandrill/8.bmp","result/mandrill/8.txt");
    toASCII("source/mandrill/16_555.bmp","result/mandrill/16_555.txt");
    toASCII("source/mandrill/16_565.bmp","result/mandrill/16_565.txt");
    toASCII("source/mandrill/24.bmp","result/mandrill/24.txt");
    toASCII("source/mandrill/32.bmp","result/mandrill/32.txt");
    contrast("source/mandrill/4.bmp","result/mandrill/4_contrast.bmp");
    contrast("source/mandrill/8.bmp","result/mandrill/8_contrast.bmp");
    contrast("source/mandrill/16_555.bmp","result/mandrill/16_555_contrast.bmp");
    contrast("source/mandrill/16_565.bmp","result/mandrill/16_565_contrast.bmp");
    contrast("source/mandrill/24.bmp","result/mandrill/24_contrast.bmp");
    contrast("source/mandrill/32.bmp","result/mandrill/32_contrast.bmp");
    gray("source/mandrill/4.bmp","result/mandrill/4_gray.bmp");
    gray("source/mandrill/8.bmp","result/mandrill/8_gray.bmp");
    gray("source/mandrill/16_555.bmp","result/mandrill/16_555_gray.bmp");
    gray("source/mandrill/16_565.bmp","result/mandrill/16_565_gray.bmp");
    gray("source/mandrill/24.bmp","result/mandrill/24_gray.bmp");
    gray("source/mandrill/32.bmp","result/mandrill/32_gray.bmp");
    invert("source/mandrill/1_mono.bmp","result/mandrill/1_invert.bmp");
    invert("source/mandrill/4.bmp","result/mandrill/4_invert.bmp");
    invert("source/mandrill/8.bmp","result/mandrill/8_invert.bmp");
    invert("source/mandrill/16_555.bmp","result/mandrill/16_555_invert.bmp");
    invert("source/mandrill/16_565.bmp","result/mandrill/16_565_invert.bmp");
    invert("source/mandrill/24.bmp","result/mandrill/24_invert.bmp");
    invert("source/mandrill/32.bmp","result/mandrill/32_invert.bmp");
    mirror("source/mandrill/1_mono.bmp","result/mandrill/1_mirror.bmp");
    mirror("source/mandrill/4.bmp","result/mandrill/4_mirror.bmp");
    mirror("source/mandrill/8.bmp","result/mandrill/8_mirror.bmp");
    mirror("source/mandrill/16_555.bmp","result/mandrill/16_555_mirror.bmp");
    mirror("source/mandrill/16_565.bmp","result/mandrill/16_565_mirror.bmp");
    mirror("source/mandrill/24.bmp","result/mandrill/24_mirror.bmp");
    mirror("source/mandrill/32.bmp","result/mandrill/32_mirror.bmp");
    
    toASCII("source/parrots/1_mono.bmp","result/parrots/1.txt");
    toASCII("source/parrots/4.bmp","result/parrots/4.txt");
    toASCII("source/parrots/8.bmp","result/parrots/8.txt");
    toASCII("source/parrots/16_555.bmp","result/parrots/16_555.txt");
    toASCII("source/parrots/16_565.bmp","result/parrots/16_565.txt");
    toASCII("source/parrots/24.bmp","result/parrots/24.txt");
    toASCII("source/parrots/32.bmp","result/parrots/32.txt");
    contrast("source/parrots/4.bmp","result/parrots/4_contrast.bmp");
    contrast("source/parrots/8.bmp","result/parrots/8_contrast.bmp");
    contrast("source/parrots/16_555.bmp","result/parrots/16_555_contrast.bmp");
    contrast("source/parrots/16_565.bmp","result/parrots/16_565_contrast.bmp");
    contrast("source/parrots/24.bmp","result/parrots/24_contrast.bmp");
    contrast("source/parrots/32.bmp","result/parrots/32_contrast.bmp");
    gray("source/parrots/4.bmp","result/parrots/4_gray.bmp");
    gray("source/parrots/8.bmp","result/parrots/8_gray.bmp");
    gray("source/parrots/16_555.bmp","result/parrots/16_555_gray.bmp");
    gray("source/parrots/16_565.bmp","result/parrots/16_565_gray.bmp");
    gray("source/parrots/24.bmp","result/parrots/24_gray.bmp");
    gray("source/parrots/32.bmp","result/parrots/32_gray.bmp");
    invert("source/parrots/1_mono.bmp","result/parrots/1_invert.bmp");
    invert("source/parrots/4.bmp","result/parrots/4_invert.bmp");
    invert("source/parrots/8.bmp","result/parrots/8_invert.bmp");
    invert("source/parrots/16_555.bmp","result/parrots/16_555_invert.bmp");
    invert("source/parrots/16_565.bmp","result/parrots/16_565_invert.bmp");
    invert("source/parrots/24.bmp","result/parrots/24_invert.bmp");
    invert("source/parrots/32.bmp","result/parrots/32_invert.bmp");
    mirror("source/parrots/1_mono.bmp","result/parrots/1_mirror.bmp");
    mirror("source/parrots/4.bmp","result/parrots/4_mirror.bmp");
    mirror("source/parrots/8.bmp","result/parrots/8_mirror.bmp");
    mirror("source/parrots/16_555.bmp","result/parrots/16_555_mirror.bmp");
    mirror("source/parrots/16_565.bmp","result/parrots/16_565_mirror.bmp");
    mirror("source/parrots/24.bmp","result/parrots/24_mirror.bmp");
    mirror("source/parrots/32.bmp","result/parrots/32_mirror.bmp");
    
    toASCII("source/pepper/1_mono.bmp","result/pepper/1.txt");
    toASCII("source/pepper/4.bmp","result/pepper/4.txt");
    toASCII("source/pepper/8.bmp","result/pepper/8.txt");
    toASCII("source/pepper/16_555.bmp","result/pepper/16_555.txt");
    toASCII("source/pepper/16_565.bmp","result/pepper/16_565.txt");
    toASCII("source/pepper/24.bmp","result/pepper/24.txt");
    toASCII("source/pepper/32.bmp","result/pepper/32.txt");
    contrast("source/pepper/4.bmp","result/pepper/4_contrast.bmp");
    contrast("source/pepper/8.bmp","result/pepper/8_contrast.bmp");
    contrast("source/pepper/16_555.bmp","result/pepper/16_555_contrast.bmp");
    contrast("source/pepper/16_565.bmp","result/pepper/16_565_contrast.bmp");
    contrast("source/pepper/24.bmp","result/pepper/24_contrast.bmp");
    contrast("source/pepper/32.bmp","result/pepper/32_contrast.bmp");
    gray("source/pepper/4.bmp","result/pepper/4_gray.bmp");
    gray("source/pepper/8.bmp","result/pepper/8_gray.bmp");
    gray("source/pepper/16_555.bmp","result/pepper/16_555_gray.bmp");
    gray("source/pepper/16_565.bmp","result/pepper/16_565_gray.bmp");
    gray("source/pepper/24.bmp","result/pepper/24_gray.bmp");
    gray("source/pepper/32.bmp","result/pepper/32_gray.bmp");
    invert("source/pepper/1_mono.bmp","result/pepper/1_invert.bmp");
    invert("source/pepper/4.bmp","result/pepper/4_invert.bmp");
    invert("source/pepper/8.bmp","result/pepper/8_invert.bmp");
    invert("source/pepper/16_555.bmp","result/pepper/16_555_invert.bmp");
    invert("source/pepper/16_565.bmp","result/pepper/16_565_invert.bmp");
    invert("source/pepper/24.bmp","result/pepper/24_invert.bmp");
    invert("source/pepper/32.bmp","result/pepper/32_invert.bmp");
    mirror("source/pepper/1_mono.bmp","result/pepper/1_mirror.bmp");
    mirror("source/pepper/4.bmp","result/pepper/4_mirror.bmp");
    mirror("source/pepper/8.bmp","result/pepper/8_mirror.bmp");
    mirror("source/pepper/16_555.bmp","result/pepper/16_555_mirror.bmp");
    mirror("source/pepper/16_565.bmp","result/pepper/16_565_mirror.bmp");
    mirror("source/pepper/24.bmp","result/pepper/24_mirror.bmp");
    mirror("source/pepper/32.bmp","result/pepper/32_mirror.bmp");
    
    toASCII("source/sailboat/1_mono.bmp","result/sailboat/1.txt");
    toASCII("source/sailboat/4.bmp","result/sailboat/4.txt");
    toASCII("source/sailboat/8.bmp","result/sailboat/8.txt");
    toASCII("source/sailboat/16_555.bmp","result/sailboat/16_555.txt");
    toASCII("source/sailboat/16_565.bmp","result/sailboat/16_565.txt");
    toASCII("source/sailboat/24.bmp","result/sailboat/24.txt");
    toASCII("source/sailboat/32.bmp","result/sailboat/32.txt");
    contrast("source/sailboat/4.bmp","result/sailboat/4_contrast.bmp");
    contrast("source/sailboat/8.bmp","result/sailboat/8_contrast.bmp");
    contrast("source/sailboat/16_555.bmp","result/sailboat/16_555_contrast.bmp");
    contrast("source/sailboat/16_565.bmp","result/sailboat/16_565_contrast.bmp");
    contrast("source/sailboat/24.bmp","result/sailboat/24_contrast.bmp");
    contrast("source/sailboat/32.bmp","result/sailboat/32_contrast.bmp");
    gray("source/sailboat/4.bmp","result/sailboat/4_gray.bmp");
    gray("source/sailboat/8.bmp","result/sailboat/8_gray.bmp");
    gray("source/sailboat/16_555.bmp","result/sailboat/16_555_gray.bmp");
    gray("source/sailboat/16_565.bmp","result/sailboat/16_565_gray.bmp");
    gray("source/sailboat/24.bmp","result/sailboat/24_gray.bmp");
    gray("source/sailboat/32.bmp","result/sailboat/32_gray.bmp");
    invert("source/sailboat/1_mono.bmp","result/sailboat/1_invert.bmp");
    invert("source/sailboat/4.bmp","result/sailboat/4_invert.bmp");
    invert("source/sailboat/8.bmp","result/sailboat/8_invert.bmp");
    invert("source/sailboat/16_555.bmp","result/sailboat/16_555_invert.bmp");
    invert("source/sailboat/16_565.bmp","result/sailboat/16_565_invert.bmp");
    invert("source/sailboat/24.bmp","result/sailboat/24_invert.bmp");
    invert("source/sailboat/32.bmp","result/sailboat/32_invert.bmp");
    mirror("source/sailboat/1_mono.bmp","result/sailboat/1_mirror.bmp");
    mirror("source/sailboat/4.bmp","result/sailboat/4_mirror.bmp");
    mirror("source/sailboat/8.bmp","result/sailboat/8_mirror.bmp");
    mirror("source/sailboat/16_555.bmp","result/sailboat/16_555_mirror.bmp");
    mirror("source/sailboat/16_565.bmp","result/sailboat/16_565_mirror.bmp");
    mirror("source/sailboat/24.bmp","result/sailboat/24_mirror.bmp");
    mirror("source/sailboat/32.bmp","result/sailboat/32_mirror.bmp");
    return 0;
}