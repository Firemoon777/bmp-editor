#include <stdio.h>
#include <plugin.h>
#include <string.h>
#include <malloc.h>
#include "bmp_writer.h"

void __init_bmp_writer(int* result, flag_t *init_data) {
	/*printf("__init_writer: Init started\n");*/
	if(init_data == NULL || result == NULL)
	{
		*result = 1;
		return;
	}
	init_data->name = (char*)malloc(3*sizeof(char));
	strcpy(init_data->name, "-ob");
	init_data->func_ptr = &to_bmp;
	init_data->argc = 1;
	init_data->instruction = "write image to 24bit bmp";
	init_data->type = FUNC_IO;
	/*printf("__init_writer: Init OK [0]\n");*/
	*result = 0;
}


