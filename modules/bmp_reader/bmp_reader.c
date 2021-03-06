#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "bmp_reader.h"

static bmp_header_t 
read_bmp_header(FILE* const input) {
	bmp_header_t header;
	fseek(input, 0, SEEK_SET);
	fread(&header, sizeof(bmp_header_t), 1, input);
	return header;
}

read_code_error_t
from_bmp(char* const input_path, struct image_t* const read) {
	FILE* input = fopen(input_path, "rb");
	bmp_header_t bmp_header;
	int i = 0, j = 0, offset;
	if(input == NULL) {
		return READ_FILE_ERROR;
	}
	bmp_header = read_bmp_header(input);
	if(bmp_header.bfType != 0x4d42) {
		return READ_UNSUPPORTED_IMAGE_FORMAT;
	}
	if(bmp_header.biBitCount != 24) {
		return READ_UNSUPPORTED_BIT_COUNT;
	}
	read->width	= bmp_header.biWidth;
	read->height = bmp_header.biHeight;
	read->pixels = (struct pixel_t*)malloc(sizeof(struct pixel_t) * bmp_header.biWidth * bmp_header.biHeight);
	offset = bmp_header.biWidth * sizeof(struct pixel_t) % 4;
	if(offset != 0) {
		offset = 4 - offset;
	} 
	fseek(input, bmp_header.bOffBits, SEEK_SET);
	for(i = 0; i < bmp_header.biHeight; i++) {
		for(j = 0; j < bmp_header.biWidth; j++) {
			fread(&read->pixels[i * (bmp_header.biWidth) + j], sizeof(struct pixel_t), 1, input);
		} 
		fseek(input, offset, SEEK_CUR);
	}
	fclose(input);
	return READ_OK;
}
