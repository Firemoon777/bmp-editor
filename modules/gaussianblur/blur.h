#ifndef _BLUR_H_
#define _BLUR_H_

#include <image.h>

typedef enum {
	TRANSFORM_OK,
	TRANSFORM_PARSE_ERROR,
	TRANSFORM_NOT_IMPLEMENTED
} bmp_transform_error_code_t; 

bmp_transform_error_code_t
gaussian_blur(struct image_t* const src, struct image_t* const result, char** argv);
#endif
