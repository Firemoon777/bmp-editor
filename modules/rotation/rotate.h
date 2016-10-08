#ifndef _BMP_TRANSFORM_H_
#define _BMP_TRANSFORM_H_

#include <image.h>

typedef enum {
	TRANSFORM_OK,
	TRANSFORM_PARSE_ERROR,
	TRANSFORM_NOT_IMPLEMENTED
} bmp_transform_error_code_t; 

bmp_transform_error_code_t
rotate(struct image_t* const src, struct image_t* const result, char* argv);

#endif
