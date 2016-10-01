#include <bmp.h>
#include <bmp_transforms.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

/* Rotation formula
 * x' = x*cos a - y*sin a
 * y' = x*sin a + y*cos a
 */
 
bmp_transform_error_code_t
rotate(struct image_t* src, double angle, struct image_t* result)
{
	int i, j, x, y, new_x, new_y;
	double v_sin, v_cos;
	struct pixel_t white_pixel = {255, 255, 255};
	if(angle < -90 || 90 < angle)
	{
		return TRANSFORM_NOT_IMPLEMENTED;
	}
	angle = angle * M_PI / 180.0f;
	v_cos = cos(angle);
	v_sin = sin(angle);
	result->width = max((int)src->width*v_cos - src->height*v_sin, (int)src->width*v_cos + src->height*v_sin);
	result->height = max((int)src->width*v_sin + src->height*v_cos, (int)-src->width*v_sin + src->height*v_cos);
	result->pixels = (struct pixel_t*)malloc(sizeof(struct pixel_t) * result->width * result->height);
	for(i = 0; i < result->height; i++)
	{
		for(j = 0; j < result->width; j++)
		{
			x = j - result->width / 2;
			y = i - result->height / 2;
			new_x = ((x*v_cos - y*v_sin)) + src->width / 2;
			new_y = ((x*v_sin + y*v_cos)) + src->height / 2; 
			if(new_x > src->width || new_y > src->height)
			{
				result->pixels[i * (result->width) + j] = white_pixel;
				continue;
			}
			
			result->pixels[i * (result->width) + j] = src->pixels[new_y * (src->width) + new_x];
		}
	}
	return TRANSFORM_OK;
}

double gauss_function(int x, int y, unsigned long radius)
{
	double r = 20.1f;
	return (1/(2*M_PI*r*r))*exp((x*x+y*y)/(2.f*r*r));
}


struct pixel_t setPixel(struct image_t* image, int x, int y, unsigned long r)
{
	struct pixel_t pixel, current;
	double red = 0, green = 0, blue = 0, f, wsum = 0;
	int i, j, count = 0;
	for(i = max(0, y - r); i <= min(image->height, y + r); i++)
	{
		for(j = max(0, x - r); j <= min(image->width, x + r); j++)
		{
			f = gauss_function(j - x, i - y, r);
			wsum += f;
			current = image->pixels[i * (image->width) + j];
			red += current.r * f;
			green += current.g * f;
			blue += current.b * f;
			count++;
		}
	}
	pixel.r = red / wsum;
	pixel.g = green / wsum;
	pixel.b = blue / wsum;
	/*printf("Current pixel: %f / %i = %f\n", red, count, red / count);*/
	return pixel;
}

bmp_transform_error_code_t
gaussian_blur(struct image_t* src, unsigned long radius, struct image_t* result)
{
	int i, j;
	result->width = src->width;
	result->height = src->height;
	result->pixels = (struct pixel_t*)malloc(sizeof(struct pixel_t) * result->height * result->width);
	for(i = 0; i < src->height; i++)
	{
		for(j = 0; j < src->width; j++)
		{
			result->pixels[i * result->width + j] = setPixel(src, j, i, radius);
		}
	}
	return TRANSFORM_OK;
}
