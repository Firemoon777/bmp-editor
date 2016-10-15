#include <stdio.h>
#include <plugin.h>
#include <dlfcn.h>
#include <stdint.h>
#include <string.h>
#include <llist.h>

register_plugin_error_code_t register_plugin(llist_t* flags, char* path, char* name) {
	void* plugin;
	int (*init_func)(int*, flag_t*);
	char init_name[256];
	int result;
	flag_t data;
	strcpy(init_name, INIT_PREFIX);
	strcat(init_name, name);
	plugin = dlopen(path, RTLD_NOW);
	if(!plugin) {
		return REGISTER_LOAD_FAIL;
	}
	init_func = (int (*)(int*, flag_t*))dlsym(plugin, init_name);
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

