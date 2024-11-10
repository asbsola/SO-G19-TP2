#ifndef LIST_H
#define LIST_H

#include <managers/memoryManager.h>
#include <stddef.h>
#include <stdint.h>

typedef struct List *ListADT;

ListADT list_init(memoryManagerADT memory_manager);

void free_list(ListADT list);

int list_add(ListADT list, void *data);

int list_remove(ListADT list, void *data);

void *list_next(const ListADT list);

size_t list_size(const ListADT list);

int list_is_empty(const ListADT list);

void *get_element(const ListADT list, void *data);

#endif
