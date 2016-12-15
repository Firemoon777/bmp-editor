#include "sepia.h"
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <plugin.h>
#include <string.h>


extern "C" void __init_sepia_cuda(int* result, plugin_action_info_t *init_data) {
	if(init_data == NULL || result == NULL)
	{ 
		*result = 1;
		return;
	}
	init_data->name = (char*)malloc(3*sizeof(char));
	strcpy(init_data->name, "-scuda");
	init_data->func_ptr = (void*)&sepia;
	init_data->instruction = (char*)"sepia effect based on CUDA";
	init_data->argc = 0;
	init_data->type = FUNC_TRANSFORM;
	*result = 0;
}

__global__ void cuda_func(pixel_t* pixels) {
	size_t i = blockIdx.x*blockDim.x + threadIdx.x;
	static const float c[3][3] = {
		{ .393f, .769f, .189f },
		{ .349f, .686f, .168f },
		{ .272f, .543f, .131f } };
	struct pixel_t old = pixels[i];
	
	int r = old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2];
	int g = old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2];
	int b = old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2];
	
	pixels[i].r = (unsigned char)min(255, r);
	pixels[i].g = (unsigned char)min(255, g);
	pixels[i].b = (unsigned char)min(255, b);
}

/*static struct pixel_t setPixel(struct image_t* const image, int x, int y) {
	static const float c[3][3] = {
		{ .393f, .769f, .189f },
		{ .349f, .686f, .168f },
		{ .272f, .543f, .131f } };
	struct pixel_t const old = image->pixels[y * (image->width) + x];
	struct pixel_t pixel;
	
	pixel.r = sat(old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]);
	pixel.g = sat(old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]);
	pixel.b = sat(old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]);
	return  pixel;
}*/

bmp_transform_error_code_t
sepia(struct image_t* const src, struct image_t* const result, char** argv) {
	size_t N = src->width*src->height;
	clock_t begin, end;
	double time_spent;
	struct pixel_t* dev_pixels, dev_new;
	result->width = src->width;
	result->height = src->height;
	result->pixels = (struct pixel_t*)malloc(sizeof(struct pixel_t) * N);
	begin = clock();
	cudaMalloc(&dev_pixels,  N*sizeof(struct pixel_t));
	cudaMemcpy(dev_pixels, src->pixels, N*sizeof(struct pixel_t), cudaMemcpyHostToDevice);
	cuda_func<<<(N+255) / 256, 256>>>(dev_pixels);
	cudaMemcpy(result->pixels, dev_pixels, N*sizeof(struct pixel_t), cudaMemcpyDeviceToHost);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("CUDA sepia time: %f\n", time_spent);
	return TRANSFORM_OK;
}
