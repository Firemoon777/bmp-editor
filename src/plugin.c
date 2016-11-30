#include <stdio.h>
#include <plugin.h>
#include <dlfcn.h>
#include <stdint.h>
#include <string.h>
#include <llist.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

plugin_register_error_code_t
plugin_register(llist_t* flags, char* path, char* name) {
	void* plugin;
	int (*init_func)(int*, plugin_action_info_t*);
	char init_name[256];
	int result;
	plugin_action_info_t data;
	strcpy(init_name, INIT_PREFIX);
	strcat(init_name, name);
	plugin = dlopen(path, RTLD_NOW);
	if(!plugin) {
		return REGISTER_LOAD_FAIL;
	}
	init_func = (int (*)(int*, plugin_action_info_t*))dlsym(plugin, init_name);
	if(!init_func) {
		dlclose(plugin);
		return REGISTER_INIT_NOT_FOUND;
	}
	init_func(&result, &data);
	if(result) {
		dlclose(plugin);
		return REGISTER_INIT_NON_ZERO;
	}
	if(llist_add(flags, data)) {
		dlclose(plugin);
		return REGISTER_FLAG_CONFLICT;
	}
	return REGISTER_OK;
}

plugin_register_error_code_t 
plugin_search_in_dir(llist_t* flags, char* dir) {
	DIR *dp;
    struct dirent *entry;
	char* plugin_name = (char*)malloc(255*sizeof(char));
	char* plugin_file_name = (char*)malloc(255*sizeof(char));	
	plugin_register_error_code_t err;
	
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
				err = plugin_register(flags, plugin_file_name, plugin_name);
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
	return REGISTER_OK;
}

