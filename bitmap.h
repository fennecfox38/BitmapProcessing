#ifndef __BITMAP_H__

#define __BITMAP_H__

#define BM 0x4D42   //Read "BM" by 2-byte of LittleEndian (B: 0x42, M: 0x4D)
#define BI_SIZE 40      // size of BITMAPINFOHEADER is 40
#define BI_RGB 0        // biCompression default value: Not Compressed
#define BI_BITFIELDS 3  // biCompression value: Manipulated by bitfield
#define PIXEL_ALIGN  4  // Pixel Array will be aligned by 4

#pragma pack(push, 1)                // Align structure by 1-byte

typedef struct _BITMAPFILEHEADER{    // BMP file header Structure
    unsigned short type;           // The File Type(aka Magic Number), must be BM
    unsigned int   size;           // This BMP File Size, in bytes
    unsigned short reserved1;      // Reserved
    unsigned short reserved2;      // Reserved
    unsigned int   offBits;        // The offset to Bitmap bits(Pixel Array)
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER{    // BMP info header Structure(DIB header)
    unsigned int   size;           // The size(byte) of this Structure
    int            width;          // The Width of Bitmap
    int            height;         // The Height of Bitmap
    unsigned short planes;         // The # of planes (must be set to 1)
    unsigned short bitCount;       // The # of bits per pixel (24 for 24bit bitmap)
    unsigned int   compression;    // The type of compression for bottom-up bitmap
    unsigned int   sizeImage;      // The size(byte) of image data only (Pixel Array)
    int            xPelsPerMeter;  // The horizonatal resolution (pixel per meter)
    int            yPelsPerMeter;  // The vertical resolution (pixel per meter)
    unsigned int   clrUsed;        // The # of color indexed in color table.
    unsigned int   clrImportant;   // The # of color required for displaying.
} BITMAPINFOHEADER;

/*typedef struct _BITMAPV4HEADER{    // BMP info header Structure(DIB header)
    unsigned int   size;           // The size(byte) of this Structure
    int            width;          // The Width of Bitmap
    int            height;         // The Height of Bitmap
    unsigned short planes;         // The # of planes (must be set to 1)
    unsigned short bitCount;       // The # of bits per pixel (24 for 24bit bitmap)
    unsigned int   compression;    // The type of compression for bottom-up bitmap
    unsigned int   sizeImage;      // The size(byte) of image data only (Pixel Array)
    int            xPelsPerMeter;  // The horizonatal resolution (pixel per meter)
    int            yPelsPerMeter;  // The vertical resolution (pixel per meter)
    unsigned int   clrUsed;        // The # of color indexed in color table.
    unsigned int   clrImportant;   // The # of color required for displaying.
    //*******************same as BITMAPINFOHEADER until here*******************
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

typedef unsigned short RGB16;      // 2byte of 16bit bitmap color data carrier
#define R555(P) (((P)&0x7C00)>>10) // read Red value from RGB16 of 16bit 555
#define G555(P) (((P)&0x3E0)>>5)   // read Green value from RGB16 of 16bit 555
#define B555(P) ((P)&0x1F)         // read Blue value from RGB16 of 16bit 555
#define RGB555(R,G,B) (((R)<<10)|((G)<<5)|(B))  // write RGB16 from each channel value of 16bit 555
#define R565(P) (((P)&0xF800)>>11) // read Red value from RGB16 of 16bit 565
#define G565(P) (((P)&0x7E0)>>5)   // read Green value from RGB16 of 16bit 565
#define B565(P) ((P)&0x1F)         // read Blue value from RGB16 of 16bit 565
#define RGB565(R,G,B) (((R)<<11)|((G)<<5)|(B))  // write RGB16 from each channel value of 16bit 565

typedef struct _RGBTRIPLE{
    unsigned char rgbtBlue;
    unsigned char rgbtGreen;
    unsigned char rgbtRed;
} RGBTRIPLE;

typedef struct _RGBQUAD{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbAlpha;
} RGBQUAD;

#pragma pack(pop)

typedef struct{
    BITMAPFILEHEADER bf;        // Bitmap File Header
    BITMAPINFOHEADER bi;        // Bitmap Info Header
    unsigned char *extra, *data;  // (intermediate extra area), (Pointer to Pixel Array Data)
    int sizData, sizPxl, padding;
    // (size of Image Data only (Pixel Array)), (Bytes per Pixel), (Padding size when width is not multiple of 4)
} IMAGE;

int readImage(IMAGE* img,char* fileName);
int writeImage(IMAGE* img, char* fileName);
void freeImage(IMAGE *img);
int toASCII(char* src, char* dst);
int gray(char* src, char* dst);
int invert(char* src, char* dst);
int mirror(char* src, char* dst);

#endif