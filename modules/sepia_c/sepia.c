#include "sepia.h"
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

static unsigned char sat( uint64_t x) {
	if (x < 256) return x;
	return 255;
}

static struct pixel_t setPixel(struct image_t* const image, int x, int y) {
	static const float c[3][3] = {
		{ .393f, .769f, .189f },
		{ .349f, .686f, .168f },
		{ .272f, .543f, .131f } };
	struct pixel_t const old = image->pixels[y * (image->width) + x];
	struct pixel_t pixel;
	
	pixel.r = sat(old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]);
	pixel.g = sat(old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]);
	pixel.b = sat(old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]);
	
	return  pixel;
}

bmp_transform_error_code_t
sepia(struct image_t* const src, struct image_t* const result, char** argv) {
	size_t i, j;
	clock_t begin, end;
	double time_spent;
	result->width = src->width;
	result->height = src->height;
	result->pixels = (struct pixel_t*)malloc(sizeof(struct pixel_t) * result->height * result->width);
	begin = clock();
	for(i = 0; i < src->height; i++) {
		for(j = 0; j < src->width; j++) {
			result->pixels[i * result->width + j] = setPixel(src, j, i);
		}
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Pure C sepia time: %f\n", time_spent);
	return TRANSFORM_OK;
}
