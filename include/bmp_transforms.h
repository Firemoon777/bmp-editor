#ifndef _BMP_TRANSFORM_H_
#define _BMP_TRANSFORM_H_

typedef enum {
	TRANSFORM_OK,
	TRANSFORM_NOT_IMPLEMENTED
} bmp_transform_error_code_t; 

bmp_transform_error_code_t
rotate(struct image_t* src, double angle, struct image_t* result);

#endif
