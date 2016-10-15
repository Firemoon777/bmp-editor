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
} func_type_t;

typedef struct {
	char* name;
	char* instruction;
	void* func_ptr;
	int argc;
	func_type_t type;
} flag_t;

typedef enum {
	REGISTER_OK = 0,
	REGISTER_LOAD_FAIL,
	REGISTER_INIT_NOT_FOUND,
	REGISTER_INIT_NON_ZERO,
	REGISTER_FLAG_CONFLICT
} register_plugin_error_code_t;

register_plugin_error_code_t register_plugin(llist_t* flags, char* path, char* name);

#endif
