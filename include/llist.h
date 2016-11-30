#ifndef _LLIST_H_
#define _LLIST_H_

#include <plugin.h>

typedef plugin_action_info_t llist_type_t;

typedef struct llist_t {
	plugin_action_info_t value;
	struct llist_t* next;
} llist_t;

llist_t* llist_create(llist_type_t value);
int llist_add(llist_t* list, llist_type_t value);
llist_t* llist_find_by_name(llist_t* list, char* name);
void llist_free(llist_t* list);
#endif
