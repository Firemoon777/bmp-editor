#ifndef _BMP_TRANSFORM_H_
#define _BMP_TRANSFORM_H_

#include <image.h>

typedef enum {
	TRANSFORM_OK,
	TRANSFORM_PARSE_ERROR,
	TRANSFORM_NOT_IMPLEMENTED,
	TRANSFORM_THREAD_ERROR
} bmp_transform_error_code_t; 

typedef struct {
	image_t* src;
	image_t* result;
	uint32_t start, end;
	double v_sin, v_cos;
} arg_struct;

bmp_transform_error_code_t
rotate(struct image_t* const src, struct image_t* const result, char** argv);



#endif
