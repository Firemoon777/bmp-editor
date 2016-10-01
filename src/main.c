#include <stdio.h>
#include <stdlib.h>
#include <bmp.h>
#include <bmp_reader.h>
#include <bmp_writer.h>
#include <bmp_transforms.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>

FILE *image_input, *image_output;

int main(int argc, char** argv) {
	struct image_t* image = (struct image_t*)malloc(sizeof(struct image_t));
	struct image_t* new_image = NULL;
	double time_spent, angle = 90.f;
	char* endPtr;
	clock_t begin, end;
	int i_src = 0, i_dest = 0, i, verbose = 0, rotation = 0, gauss = 0;
	unsigned long radius;
	if(argc < 3)
	{
		printf("Usage: %s [options] source destination\n", argv[0]);
		printf("Options:\n");
		printf("    -a n      rotate image on n degrees\n");
		printf("    -g r      gaussian blur with radius r\n");
		printf("    -v        be verbose\n");
		return 0;
	}
	for(i = 1; i < argc; i++)
	{
		if(argv[i][0] != '-')
		{
			i_src = i_dest;
			i_dest = i;
		} else if (argv[i][1] == 'a') {
			angle = strtod(argv[i+1], &endPtr);
			if(endPtr == argv[i+1])
			{
				printf("Incorrect angle\n");
				return -1;
			}
			rotation = 1;
			i++;
		} else if(argv[i][1] == 'v') {
			verbose = 1;
		} else if(argv[i][1] == 'g') {
			radius = strtoul(argv[i+1], &endPtr, 10);
			if(endPtr == argv[i+1] || argv[i+1][0] == '-')
			{
				printf("Incorrect radius\n");
				return -1;
			}
			gauss = 1;
			i++;
		}
	}
	image_input = fopen(argv[i_src], "rb");
	image_output = fopen(argv[i_dest], "wb");
	
	begin = clock();
	switch(from_bmp(image_input, image))
	{
		case READ_FILE_ERROR:
			printf("Cannot open file\n");
			return 1;
		case READ_UNSUPPORTED_IMAGE_FORMAT:
			printf("Incorrect file format.\nSupport only 24bit BMP images\n");
			return 2;
			
		case READ_UNSUPPORTED_BIT_COUNT:
			printf("Incorrect bitCount in BMP.\nSupport only 24bit BMP images\n");
			return 3;
		case READ_OK:
			if(verbose)
				printf("Input size: %ix%ix24\n", image->width, image->height);
			break;
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	if(verbose)
		printf("Reading time: %f seconds\n", time_spent);
		
	if(rotation)
	{
		begin = clock();
		new_image = (struct image_t*)malloc(sizeof(struct image_t));
		switch(rotate(image, angle, new_image))
		{
			case TRANSFORM_NOT_IMPLEMENTED:
				printf("Avaliable rotation angle: [-90; 90] degrees\n");
				return 4;
			case TRANSFORM_OK:						
				free(image->pixels);
				free(image);
				image = new_image;
				new_image = NULL;	
				break;
		}
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		if(verbose)
			printf("Rotation time: %f seconds\n", time_spent);
	}
	
	if(gauss)
	{
		begin = clock();
		new_image = (struct image_t*)malloc(sizeof(struct image_t));
		switch(gaussian_blur(image, radius, new_image))
		{
			case TRANSFORM_NOT_IMPLEMENTED:
				printf("Stop. What?\n");
				return 4;
			case TRANSFORM_OK:						
				free(image->pixels);
				free(image);
				image = new_image;
				new_image = NULL;	
				break;
		}
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		if(verbose)
			printf("Gauss time: %f seconds\n", time_spent);
	}
	
	begin = clock();
	if(verbose)
		printf("Output size: %ix%ix24\n", image->width, image->height);
	switch(to_bmp(image_output, image))
	{
		case WRITE_FILE_ERROR:
			printf("Cannot open file\n");
			return 1;
			
		case WRITE_OK:
			break;
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	if(verbose)
		printf("Writing time: %f seconds\n", time_spent);
	
	
	fclose(image_input);
	fclose(image_output);
	free(image->pixels);
	free(image);
	return 0;
}
