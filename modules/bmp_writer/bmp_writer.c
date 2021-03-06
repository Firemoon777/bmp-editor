#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "bmp_writer.h"

write_code_error_t to_bmp(char* const output_path, struct image_t* const image) {
	int i, j;
	bmp_header_t header;
	int offset;
	FILE* output = fopen(output_path, "wb");
	if(output == NULL) {
		return WRITE_FILE_ERROR;
	}
	offset = image->width * sizeof(struct pixel_t) % 4;
	if(offset != 0) {
		offset = 4 - offset;
	} 
	header.bfType = 0x4d42;
	header.bfileSize = 54 + (image->width*3 + offset) * image->height;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bOffBits = 54;
	header.biSize = 40;
	header.biWidth = image->width;
	header.biHeight = image->height;
	header.biPlanes = 1;
	header.biBitCount = 24;
	header.biCompression = 0;
	header.biSizeImage = 0;
	header.biXPelsPerMeter = 0xb13;
	header.biYPelsPerMeter = 0xb13;
	header.biClrUsed = 0;
	header.biClrImportant = 0;
	fwrite(&header, sizeof(bmp_header_t)*2, 1, output);
	fseek(output, 54, SEEK_SET);
	for(i = 0; i < image->height; i++) {
		for(j = 0; j < image->width; j++) {
			fwrite(&image->pixels[i*(image->width) + j], sizeof(struct pixel_t), 1, output);
		}
		fseek(output, offset, SEEK_CUR); 
	}
	fclose(output);
	return WRITE_OK;
}
