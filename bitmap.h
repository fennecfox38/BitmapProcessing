#ifndef __BITMAP_H__

#define __BITMAP_H__

#define BM 0x4D42   //Read "BM" by 2-byte of LittleEndian (B: 0x42, M: 0x4D)
#define BI_SIZE 40      // size of BITMAPINFOHEADER is 40
#define BI_RGB 0        // biCompression default value: Not Compressed
#define BI_BITFIELDS 3  // biCompression value: Manipulated by bitfield
#define PIXEL_ALIGN  4  // Pixel Array will be aligned by 4

#pragma pack(push, 1)               // Align structure by 1-byte

typedef struct _BITMAPFILEHEADER{   // BMP file header Structure
    unsigned short type;            // The File Type(aka Magic Number), must be BM
    unsigned int   size;            // This BMP File Size, in bytes
    unsigned short reserved1;       // Reserved
    unsigned short reserved2;       // Reserved
    unsigned int   offBits;         // The offset to Bitmap bits(Pixel Array)
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER{   // BMP info header Structure(DIB header)
    unsigned int   size;            // The size(byte) of this Structure
    int            width;           // The Width of Bitmap
    int            height;          // The Height of Bitmap
    unsigned short planes;          // The # of planes (must be set to 1)
    unsigned short bitCount;        // The # of bits per pixel (24 for 24 bitmap)
    unsigned int   compression;     // The type of compression for bottom-up bitmap
    unsigned int   sizeImage;       // The size(byte) of image data only (Pixel Array)
    int            xPelsPerMeter;   // The horizonatal resolution (pixel per meter)
    int            yPelsPerMeter;   // The vertical resolution (pixel per meter)
    unsigned int   clrUsed;         // The # of color indexed in color table.
    unsigned int   clrImportant;    // The # of color required for displaying.
} BITMAPINFOHEADER;

/*typedef struct _BITMAPV4HEADER{   // BMP info header Structure(DIB header)
    unsigned int   size;            // The size(byte) of this Structure
    int            width;           // The Width of Bitmap
    int            height;          // The Height of Bitmap
    unsigned short planes;          // The # of planes (must be set to 1)
    unsigned short bitCount;        // The # of bits per pixel (24 for 24 bitmap)
    unsigned int   compression;     // The type of compression for bottom-up bitmap
    unsigned int   sizeImage;       // The size(byte) of image data only (Pixel Array)
    int            xPelsPerMeter;   // The horizonatal resolution (pixel per meter)
    int            yPelsPerMeter;   // The vertical resolution (pixel per meter)
    unsigned int   clrUsed;         // The # of color indexed in color table.
    unsigned int   clrImportant;    // The # of color required for displaying.
    *******************same as BITMAPINFOHEADER until here*******************
    DWORD        bV4RedMask;
    DWORD        bV4GreenMask;
    DWORD        bV4BlueMask;
    DWORD        bV4AlphaMask;
    DWORD        bV4CSType;
    CIEXYZTRIPLE bV4Endpoints;
    DWORD        bV4GammaRed;
    DWORD        bV4GammaGreen;
    DWORD        bV4GammaBlue;
}BITMAPV4HEADER;*/

// Read and Write 1 bit of 'VAL' at 'X'th or 'BIT'th bit, by Bitwise Operation.
#define GET1(VAL,X) (((VAL)&(0x80>>((X)%8)))?1:0)//>>(7-((X)%8)))
#define SET1(BYTE,BIT,VAL) if(VAL){*(BYTE)|=((0x80)>>(BIT%8));} else{*(BYTE)&=~((0x80)>>(BIT%8));}

// Read and Write 4 bit of 'VAL' at 'X'th or 'PART'th part,
// by accessing member variable in Structure Bitfield.
#define GET4(VAL,X) (((VAL)&(((X)&1)?0x0F:0xF0))>>(((X)&1)?0:4))
#define SET4(BYTE,PART,VAL) if((PART)&1){((BYTE)->_1)=(VAL);} else{((BYTE)->_0)=(VAL);}
typedef struct{
    unsigned char _1 : 4;       // Less Significant 4 Bit
    unsigned char _0 : 4;       // More Significant 4 Bit
} RGB4;

typedef struct{
    unsigned short blue : 5;    // Blue Channel
    unsigned short green : 5;   // Green Channel
    unsigned short red : 5;     // Red Channel
    unsigned short alpha : 1;   // Alpha Channel
    // Alpha channel would be Reserved if transparent is not supported.
} RGB16_555;

typedef struct{
    unsigned short blue : 5;    // Blue Channel
    unsigned short green : 6;   // Green Channel
    unsigned short red : 5;     // Red Channel
} RGB16_565;

typedef struct _RGBTRIPLE{
    unsigned char rgbtBlue;     // Blue Channel
    unsigned char rgbtGreen;    // Green Channel
    unsigned char rgbtRed;      // Red Channel
} RGBTRIPLE;

typedef struct _RGBQUAD{
    unsigned char rgbBlue;      // Blue Channel
    unsigned char rgbGreen;     // Green Channel
    unsigned char rgbRed;       // Red Channel
    unsigned char rgbAlpha;     // Alpha Channel
    // Alpha channel would be Reserved if transparent is not supported.
} RGBQUAD;

#pragma pack(pop)               // Disable alignment declared previously.

typedef struct{     // Container for Abstracted info and data of BMP file
    BITMAPFILEHEADER bf;        // Bitmap File Header
    BITMAPINFOHEADER bi;        // Bitmap Info Header
    unsigned char *extra;       // Intermediate extra area
    // contains color table or empty
    // (Bit Mask for each channel if it is compressed by BI_BITFIELD)
    unsigned char *data;        // Pointer to Pixel Array Data
    // contains index to color table or color value itself.
    int sizData, sizRow;
    // (size of Image Data only (Pixel Array)), (size(byte) of one row including padding)
} IMAGE;

// Macro IDX is finally equivalent to 'index of pixel(X,Y), in Pixel Array Data'
#define IDX(X,Y,SIZROW,BITCOUNT) (((Y)*(SIZROW)) + ((X)*(BITCOUNT)/8))

// Macro PIXEL is finally equivalent to 'value of pixel(X,Y), in Pixel Array Data'
#define PIXEL(IMG,X,Y) (((IMG).data)+IDX(X,Y,((IMG).sizRow),((IMG).bi.bitCount)))

// Below are functions in Bitmap Library
int readImage(IMAGE* img,char* fileName);
int writeImage(IMAGE* img, char* fileName);
void freeImage(IMAGE *img);
void swap1(void* a, void* b, int bitA, int bitB);
void swap4(void* a, void* b, int partA, int partB);
void swap8(void* a, void* b);
void swap16(void* a, void* b);
void swap24(void* a, void* b);
void swap32(void* a, void* b);
int toASCII(char* src, char* dst);
int gray(char* src, char* dst);
int invert(char* src, char* dst);
int mirror(char* src, char* dst);
int contrast(char* src,char* dst);

#endif