#include <llist.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <plugin.h>

llist_t* llist_create(llist_type_t value) {
	llist_t* item = (llist_t*)malloc(sizeof(llist_t));
	item->next = NULL;
	item->value = value;
	return item;
}

int llist_add(llist_t* list, llist_type_t value) {
	while(list != NULL) {
		if(strcmp(list->value.name, value.name) == 0)
			return 1;
		if(list->next != NULL) {
			list = list->next;
		} else {
			break;
		}
	}
	list->next = llist_create(value);
	return 0;
}

llist_t* llist_find_by_name(llist_t* list, char* name) {
	while(list != NULL) {
		if(strcmp(list->value.name, name) == 0)
			return list;
		list = list->next;
	}
	return NULL;
}

void llist_free(llist_t* list)
{
	while(list != NULL)
	{
		llist_free(list->next);
		free(list);
	}
}


