#include <stdio.h>
#include <plugin.h>
#include <llist.h>
#include <stdlib.h>
#include <image.h>
#include <stdint.h>
#include <time.h>


void print_version() {
	printf("bmp-editor v2.0.0\n");
}

void print_help(llist_t* f) {
	printf("Usage:\n");
	printf("    option  argc \n");
	while(f != NULL) {
		printf("%10s %4i  %s\n", f->value.name, f->value.argc, f->value.instruction);
		f = f->next;
	}
}

llist_t* register_flags_init() {
	plugin_action_info_t info;
	llist_t* registered_flags;
	
	info.name = "-v";
	info.type = FUNC_VOID;
	info.func_ptr = &print_version;
	info.argc = 0;
	info.instruction = "show version";
	registered_flags = llist_create(info);
	
	info.name = "--help";
	info.type = FUNC_HELP;
	info.argc = 0;
	info.func_ptr = &print_help;
	info.instruction = "show this help";
	llist_add(registered_flags, info);
	return registered_flags;
}

int main(int argc, char** argv) {
	struct image_t* image = (struct image_t*)malloc(sizeof(struct image_t));
	struct image_t* new_image = NULL;
	llist_t* registered_flags, *current;
	plugin_register_error_code_t err;
	int i = 0;
	int result;
	
	registered_flags = register_flags_init();
	
	err = plugin_search_in_dir(registered_flags, "./plugins");
	if(err != REGISTER_OK) {
		return 1;
	}

	for(i = 1; i < argc; i++) {
		current = llist_find_by_name(registered_flags, argv[i]);
		if(current == NULL) {
			printf("Illegal option: %s\n", argv[i]);
			print_help(registered_flags);
			return 2;
		}
		result = 0;
		switch(current->value.type) {
			case FUNC_IO:
				result = ((int(*)(char*, image_t*))current->value.func_ptr)(argv[i+1], image);
				break;
			case FUNC_TRANSFORM:
				new_image = (struct image_t*)malloc(sizeof(struct image_t));
				result = ((int(*)(image_t*, image_t*, char**))current->value.func_ptr)(image, new_image, &argv[i+1]);
				free(image);
				image = new_image;
				new_image = NULL;
				break;
			case FUNC_HELP:
				((void(*)(llist_t*))current->value.func_ptr)(registered_flags);
				break;
			case FUNC_VOID:
				((void(*)())current->value.func_ptr)();
				break;
		}
		if(result) {
			printf("flag %s exits with code %i\n", argv[i], result);
			return 128 +  result;
		}
		i += current->value.argc;
	}
	return 0;
}
