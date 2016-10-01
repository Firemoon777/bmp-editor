#ifndef _BMP_H_
#define _BMP_H_

#include <stdint.h>

typedef struct {
	uint16_t bfType;
	uint32_t bfileSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bOffBits;
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} bmp_header_t;

union bmp_union_t {
	bmp_header_t header;
	char buff[432];
};

struct pixel_t { 
	unsigned char b,g,r; 
};

struct image_t {
	uint32_t width, height;
	struct pixel_t* pixels;
};

#endif
