#include <stdio.h>
#include <plugin.h>
#include <llist.h>
#include <stdlib.h>
#include <image.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>

void print_version() {
	printf("bmp-editor v2.0.0\n");
}

FILE *image_input, *image_output;

int main(int argc, char** argv) {
	struct image_t* image = (struct image_t*)malloc(sizeof(struct image_t));
	struct image_t* new_image = NULL;
	llist_t* registered_flags, *current;
	/*register_plugin_error_code_t err;*/
	flag_t version;
	int i = 0;
	version.name = "-v";
	version.type = FUNC_UNKNOWN;
	version.func_ptr = &print_version;
	registered_flags = llist_create(version);
	/* TODO:
	 * Dynamic check plugins/ *.so
	 * Error parsing
	 */
	register_plugin(registered_flags, "./plugins/bmp_reader.so", "bmp_reader");
	register_plugin(registered_flags, "./plugins/bmp_writer.so", "bmp_writer");
	register_plugin(registered_flags, "./plugins/blur.so", "blur");
	register_plugin(registered_flags, "./plugins/rotate.so", "rotate");
	for(i = 1; i < argc; i++) {
		current = llist_find_by_name(registered_flags, argv[i]);
		if(current == NULL) {
			printf("Unknown option: %s\n", argv[i]);
			return 1;
		}
		switch(current->value.type)
		{
			case FUNC_IO:
				printf("IO\n");
				printf("result: %i\n", ((int(*)(char*, image_t*))current->value.func_ptr)(argv[i+1], image));
				break;
			case FUNC_TRANSFORM:
				printf("TRANSFORM\n");
				new_image = (struct image_t*)malloc(sizeof(struct image_t));
				printf("result: %i\n", ((int(*)(image_t*, image_t*, char*))current->value.func_ptr)(image, new_image, argv[i+1]));
				free(image);
				image = new_image;
				new_image = NULL;
				break;
			case FUNC_UNKNOWN:
				printf("WAT\n");
				break;
		}
		i += current->value.argc;
	}
	/* TODO:
	 * Memory free
	 * Dynamic dlclose based on all loaded and registered plugins
	 */
	return 0;
}
