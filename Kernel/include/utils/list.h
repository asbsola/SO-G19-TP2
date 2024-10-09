#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>
#include <managers/memoryManager.h>

typedef struct List List;
typedef struct ListNode ListNode;

struct ListNode {
    void * data;              
    ListNode *next;
};

struct List {
    ListNode *head;
    ListNode *last;
    size_t size;
};


List *list_init(memoryManagerADT memory_manager);

void free_list(memoryManagerADT memory_manager, List *list);

uint8_t list_add(memoryManagerADT memory_manager, List *list, void *data);

uint8_t list_remove(memoryManagerADT memory_manager, List *list, void *data);

size_t list_size(memoryManagerADT memory_manager, const List *list);

uint8_t list_is_empty(memoryManagerADT memory_manager, const List *list);


#endif