#include <stdio.h>
#include <plugin.h>
#include <string.h>
#include <malloc.h>
#include "rotate.h"

void __init_rotate(int* result, plugin_action_info_t *init_data) {
	if(init_data == NULL || result == NULL)
	{ 
		*result = 1;
		return;
	}
	init_data->name = (char*)malloc(3*sizeof(char));
	strcpy(init_data->name, "-r");
	init_data->instruction = "rotate image on arg1 degrees. arg1 can be between -90 and 90";
	init_data->func_ptr = &rotate;
	init_data->argc = 1;
	init_data->type = FUNC_TRANSFORM;
	*result = 0;
}


