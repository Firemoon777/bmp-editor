#ifndef _BMP_READER_H_
#define _BMP_READER_H_

typedef enum {
	READ_OK = 0,
	READ_FILE_ERROR,
	READ_UNSUPPORTED_IMAGE_FORMAT,
	READ_UNSUPPORTED_BIT_COUNT
} read_code_error_t;

read_code_error_t from_bmp(FILE*, struct image_t* const);

#endif
