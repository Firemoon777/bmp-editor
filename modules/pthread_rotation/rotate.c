#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "rotate.h"

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

/* Rotation formula
 * x' = x*cos a - y*sin a
 * y' = x*sin a + y*cos a
 */
 
static void *background_rotate(void *arguments) {
	arg_struct *arg = (arg_struct*)arguments;
	struct pixel_t white_pixel = {255, 255, 255};
	int32_t i, j, x, y, new_x, new_y;
	image_t* const src = arg->src;
	image_t* const result = arg->result;
	double v_sin = arg->v_sin;
	double v_cos = arg->v_cos;
	
	/*printf("THREAD FROM %u TO %u\n", arg->start, arg->end);*/
	
	for(i = arg->start; i < arg->end; i++) {
		for(j = 0; j < result->width; j++) {
			x = j - result->width / 2;
			y = i - result->height / 2;
			new_x = ((x*v_cos - y*v_sin)) + src->width / 2;
			new_y = ((x*v_sin + y*v_cos)) + src->height / 2; 
			if(new_x >= src->width || new_y >= src->height) {
				result->pixels[i] = white_pixel;
				/*printf("IP: %i %i\n", new_x, new_y);*/
				continue;
			}
			/*printf("PP: %i %i\n", new_x, new_y);*/
			result->pixels[i * result->width + j] = src->pixels[new_y * (src->width) + new_x];
		}
	}
	return NULL;
}
 
bmp_transform_error_code_t
rotate(struct image_t* const src, struct image_t* const result, char** argv) {
	uint32_t thread_count, i, pixels_per_thread;
	double angle, v_cos, v_sin;
	char* end_ptr;
	
	pthread_t* threads;
	arg_struct *args;
	
	angle = strtod(argv[0], &end_ptr);
	
	if(end_ptr == argv[0]) {
		return TRANSFORM_PARSE_ERROR;
	}
	if(angle < -90 || 90 < angle) {
		return TRANSFORM_NOT_IMPLEMENTED;
	}
		
	angle = angle * M_PI / 180.0f;
	v_cos = cos(angle);
	v_sin = sin(angle);
	
	thread_count = strtoul(argv[1], &end_ptr, 10);
	if(end_ptr == argv[1]) {
		return TRANSFORM_PARSE_ERROR;
	}
	
	result->width = max((int)src->width*v_cos - src->height*v_sin, (int)src->width*v_cos + src->height*v_sin);
	result->height = max((int)src->width*v_sin + src->height*v_cos, (int)-src->width*v_sin + src->height*v_cos);
	result->pixels = (struct pixel_t*)malloc(sizeof(struct pixel_t) * result->width * result->height);

	threads = (pthread_t*)malloc(thread_count*sizeof(pthread_t));
	args = (arg_struct*)malloc(thread_count*sizeof(arg_struct));
	pixels_per_thread = result->height / thread_count;
	
	printf("Size: %u %u\n", result->width, result->height);
	
	for(i = 0; i < thread_count; i++) {
		args[i].v_cos = v_cos;
		args[i].v_sin = v_sin;
		args[i].src = src;
		args[i].result = result;
		args[i].start = pixels_per_thread * i;
		args[i].end = pixels_per_thread * (i + 1);
		if(pthread_create(&threads[i], NULL, background_rotate, &args[i])) {
			return TRANSFORM_THREAD_ERROR;
		}
	}
	for(i = 0; i < thread_count; i++) {
		pthread_join(threads[i], NULL);
	}
	return TRANSFORM_OK;
}
