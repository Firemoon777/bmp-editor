#include <stdio.h>
#include <plugin.h>
#include <llist.h>
#include <stdlib.h>
#include <image.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <time.h>

void print_version() {
	printf("bmp-editor v2.0.0\n");
}

void print_help(llist_t* f) {
	print_version();
	printf("Usage:\n");
	printf("    option  argc \n");
	while(f != NULL) {
		printf("%10s %4i  %s\n", f->value.name, f->value.argc, f->value.instruction);
		f = f->next;
	}
}

FILE *image_input, *image_output;

int main(int argc, char** argv) {
	struct image_t* image = (struct image_t*)malloc(sizeof(struct image_t));
	struct image_t* new_image = NULL;
	llist_t* registered_flags, *current;
	register_plugin_error_code_t err;
	flag_t version;
	int i = 0;
	int result;
	/*struct timespec mt1, mt2; */
	
	DIR *dp;
    struct dirent *entry;
	char dir[] = "./plugins/";
	char* plugin_name = (char*)malloc(255*sizeof(char));
	char* plugin_file_name = (char*)malloc(255*sizeof(char));	
	
	version.name = "-v";
	version.type = FUNC_VOID;
	version.func_ptr = &print_version;
	version.argc = 0;
	version.instruction = "show version";
	registered_flags = llist_create(version);
	
	version.name = "--help";
	version.type = FUNC_HELP;
	version.argc = 0;
	version.func_ptr = &print_help;
	version.instruction = "show this help";
	llist_add(registered_flags, version);
	
	dp = opendir(dir);
	if(dp == NULL) {
		fprintf(stderr, "Cannot open directory %s\n", dir);
		return 2;
	}
	chdir(dir);
	entry = readdir(dp);
	while(entry != NULL) {
		if(strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name) != 0) {
			if(strstr(entry->d_name, ".so")) {
				strncpy(plugin_name, entry->d_name, strlen(entry->d_name)-3);
				plugin_name[strlen(entry->d_name)-3] = 0;
				strcpy(plugin_file_name, "./");
				strcat(plugin_file_name, entry->d_name);
				err = register_plugin(registered_flags, plugin_file_name, plugin_name);
				switch(err) {
					case REGISTER_OK:
						fprintf(stderr, "%s.so registered\n", plugin_name);
						break;
					case REGISTER_LOAD_FAIL:
						fprintf(stderr, "Cannot open %s.so\n", plugin_name);
						break;
					case REGISTER_INIT_NOT_FOUND:
						fprintf(stderr, "Cannot found %s%s in %s.so\n", INIT_PREFIX, plugin_name, plugin_name);
						break;
					case REGISTER_INIT_NON_ZERO:
						fprintf(stderr, "Init of %s.so exits with non-zero code\n", plugin_name);
						break;
					case REGISTER_FLAG_CONFLICT:
						fprintf(stderr, "Flag from %s.so conflicts with one of already registered flags\n", plugin_name);
						break;
				}
			}
		}
		entry = readdir(dp);
	}
	chdir("..");

	for(i = 1; i < argc; i++) {
		current = llist_find_by_name(registered_flags, argv[i]);
		if(current == NULL) {
			printf("Unknown option: %s\n", argv[i]);
			return 1;
		}
		result = 0;
		switch(current->value.type) {
			case FUNC_IO:
				result = ((int(*)(char*, image_t*))current->value.func_ptr)(argv[i+1], image);
				break;
			case FUNC_TRANSFORM:
				new_image = (struct image_t*)malloc(sizeof(struct image_t));
				/*clock_gettime (CLOCK_REALTIME, &mt1);*/
				result = ((int(*)(image_t*, image_t*, char**))current->value.func_ptr)(image, new_image, &argv[i+1]);
				/*clock_gettime (CLOCK_REALTIME, &mt2);*/
				/*printf("Time spend: %ld ns\n", 1000000000*(mt2.tv_sec - mt1.tv_sec)+(mt2.tv_nsec - mt1.tv_nsec));*/
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
		}
		i += current->value.argc;
	}
	return 0;
}
