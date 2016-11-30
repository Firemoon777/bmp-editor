#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <image.h>

#define INIT_PREFIX "__init_"

typedef struct llist_t llist_t;
 
typedef enum {
	FUNC_IO = 0,		/* int(*)(char*, image_t*) */
	FUNC_TRANSFORM, 	/* int(*)(image_t* src, image_t* result, char** argv)*/
	FUNC_VOID,			/* void(*)(void) */
	FUNC_HELP			/* void(*)(llist_t*) */
} plugin_func_type_t;

typedef struct {
	char* name;
	char* instruction;
	void* func_ptr;
	int argc;
	plugin_func_type_t type;
} plugin_action_info_t;

typedef enum {
	REGISTER_OK = 0,
	REGISTER_LOAD_FAIL,
	REGISTER_INIT_NOT_FOUND,
	REGISTER_INIT_NON_ZERO,
	REGISTER_FLAG_CONFLICT
} plugin_register_error_code_t;

plugin_register_error_code_t plugin_register(llist_t* flags, char* path, char* name);
plugin_register_error_code_t plugin_search_in_dir(llist_t* flags, char* dir);

#endif
