#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdint.h>

struct pixel_t { 
	unsigned char b,g,r; 
};

typedef struct image_t {
	uint32_t width, height;
	struct pixel_t* pixels;
} image_t;

#endif
