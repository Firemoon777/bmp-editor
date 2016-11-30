#include <stdio.h>
#include <plugin.h>
#include <string.h>
#include <malloc.h>
#include "sepia.h"

void __init_sepia_asm(int* result, plugin_action_info_t *init_data) {
	if(init_data == NULL || result == NULL)
	{ 
		*result = 1;
		return;
	}
	init_data->name = (char*)malloc(3*sizeof(char));
	strcpy(init_data->name, "-sasm");
	init_data->func_ptr = &sepia;
	init_data->instruction = "sepia effect based on SSE";
	init_data->argc = 0;
	init_data->type = FUNC_TRANSFORM;
	*result = 0;
}


