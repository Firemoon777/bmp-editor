#include <stdio.h>
#include <plugin.h>
#include <string.h>
#include <malloc.h>
#include "bmp_reader.h"

void __init_bmp_reader(int* result, plugin_action_info_t *init_data) {
	/*printf("__init_reader: Init started\n");*/
	if(init_data == NULL || result == NULL)
	{
		*result = 1;
		return;
	}
	init_data->name = (char*)malloc(3*sizeof(char));
	strcpy(init_data->name, "-ib");
	init_data->func_ptr = &from_bmp;
	init_data->instruction = "read image from 24bit bmp file";
	init_data->argc = 1;
	init_data->type = FUNC_IO;
	/*printf("__init_reader: Init OK [0]\n");*/
	*result = 0;
}


