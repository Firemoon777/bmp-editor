#ifndef _BMP_WRITER_H_
#define _BMP_WRITER_H_

typedef enum {
	WRITE_OK = 0,
	WRITE_FILE_ERROR
} write_code_error_t;

write_code_error_t to_bmp(FILE*, struct image_t* const);

#endif
