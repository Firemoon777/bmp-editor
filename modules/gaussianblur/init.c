#include <stdio.h>
#include <plugin.h>
#include <string.h>
#include <malloc.h>
#include "blur.h"

void __init_blur(int* result, flag_t *init_data) {
	/*printf("__init_blur: Init started\n");*/
	if(init_data == NULL || result == NULL)
	{ 
		*result = 1;
		return;
	}
	init_data->name = (char*)malloc(3*sizeof(char));
	strcpy(init_data->name, "-g");
	init_data->func_ptr = &gaussian_blur;
	init_data->argc = 1;
	init_data->type = FUNC_TRANSFORM;
	/*printf("__init_blur: Init OK [0]\n");*/
	*result = 0;
}


