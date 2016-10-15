#include "blur.h"
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

static double gauss_function(int x, int y, unsigned long radius) {
	double r = 20.1f;
	return (1/(2*M_PI*r*r))*exp((x*x+y*y)/(2.f*r*r));
}


static struct pixel_t setPixel(struct image_t* const image, int x, int y, unsigned long r) {
	struct pixel_t pixel, current;
	double red = 0, green = 0, blue = 0, f, wsum = 0;
	int i, j, count = 0;
	for(i = max(0, y - r); i <= min(image->height-1, y + r); i++) {
		for(j = max(0, x - r); j <= min(image->width-1, x + r); j++) {
			f = gauss_function(j - x, i - y, r);
			wsum += f;
			current = image->pixels[i * (image->width) + j];
			red += current.r * f;
			green += current.g * f;
			blue += current.b * f;
			count++;
		}
	}
	pixel.r = red / wsum;
	pixel.g = green / wsum;
	pixel.b = blue / wsum;
	return pixel;
}

bmp_transform_error_code_t
gaussian_blur(struct image_t* const src, struct image_t* const result, char** argv) {
	int i, j;
	unsigned long radius;
	char* end_ptr;
	radius = strtoul(argv[0], &end_ptr, 10);
	if(argv[0][0] == '-' || end_ptr == argv[0]) {
		return TRANSFORM_PARSE_ERROR;
	}
	result->width = src->width;
	result->height = src->height;
	result->pixels = (struct pixel_t*)malloc(sizeof(struct pixel_t) * result->height * result->width);
	for(i = 0; i < src->height; i++) {
		for(j = 0; j < src->width; j++) {
			result->pixels[i * result->width + j] = setPixel(src, j, i, radius);
		}
	}
	return TRANSFORM_OK;
}
