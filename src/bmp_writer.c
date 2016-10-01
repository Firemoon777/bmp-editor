#include <stdio.h>
#include <stdlib.h>
#include <bmp.h>
#include <bmp_writer.h>

write_code_error_t to_bmp(FILE* output, struct image_t* image)
{
	int i, j;
	union bmp_union_t file_header;
	bmp_header_t header;
	int offset;
	if(output == NULL)
	{
		return WRITE_FILE_ERROR;
	}
	offset = image->width * sizeof(struct pixel_t) % 4;
	if(offset != 0)
	{
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
	file_header.header = header;
	fwrite(&file_header.buff, sizeof(char)*2, 1, output);
	fwrite(&file_header.buff[4], sizeof(char)*52*8, 1, output);
	fseek(output, 54, SEEK_SET);
	for(i = 0; i < image->height; i++)
	{
		for(j = 0; j < image->width; j++)
		{
			fwrite(&image->pixels[i*(image->width) + j], sizeof(struct pixel_t), 1, output);
		}
		fseek(output, offset, SEEK_CUR); 
	}
	return WRITE_OK;
}
