#include <stdio.h>
#include <plugin.h>
#include <string.h>
#include <malloc.h>
#include "rotate.h"

void __init_mrotate(int* result, flag_t *init_data) {
	if(init_data == NULL || result == NULL)
	{ 
		*result = 1;
		return;
	}
	init_data->name = (char*)malloc(3*sizeof(char));
	strcpy(init_data->name, "-mr");
	init_data->instruction = "multithreading rotate image on arg1 degrees. arg1 can be between -90 and 90, arg2 is count of threads";
	init_data->func_ptr = &rotate;
	init_data->argc = 2;
	init_data->type = FUNC_TRANSFORM;
	*result = 0;
}


