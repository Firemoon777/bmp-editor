#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rotate.h"

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

/* Rotation formula
 * x' = x*cos a - y*sin a
 * y' = x*sin a + y*cos a
 */
 
bmp_transform_error_code_t
rotate(struct image_t* const src, struct image_t* const result, char** argv) {
	int i, j, x, y, new_x, new_y;
	double v_sin, v_cos, angle;
	char* end_ptr;
	struct pixel_t white_pixel = {255, 255, 255};
	angle = strtod(argv[0], &end_ptr);
	if(end_ptr == argv[0]) {
		return TRANSFORM_PARSE_ERROR;
	}
	if(angle < -90 || 90 < angle) {
		return TRANSFORM_NOT_IMPLEMENTED;
	}
	angle = angle * M_PI / 180.0f;
	v_cos = cos(angle);
	v_sin = sin(angle);
	result->width = max((int)src->width*v_cos - src->height*v_sin, (int)src->width*v_cos + src->height*v_sin);
	result->height = max((int)src->width*v_sin + src->height*v_cos, (int)-src->width*v_sin + src->height*v_cos);
	result->pixels = (struct pixel_t*)malloc(sizeof(struct pixel_t) * result->width * result->height);
	for(i = 0; i < result->height; i++) {
		for(j = 0; j < result->width; j++) {
			x = j - result->width / 2;
			y = i - result->height / 2;
			new_x = ((x*v_cos - y*v_sin)) + src->width / 2;
			new_y = ((x*v_sin + y*v_cos)) + src->height / 2; 
			if(new_x >= src->width || new_y >= src->height) {
				result->pixels[i * (result->width) + j] = white_pixel;
				continue;
			}
			result->pixels[i * (result->width) + j] = src->pixels[new_y * (src->width) + new_x];
		}
	}
	return TRANSFORM_OK;
}
