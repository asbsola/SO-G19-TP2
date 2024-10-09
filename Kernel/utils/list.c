
#include <utils/list.h>


List *list_init(memoryManagerADT memory_manager) {
    List *list = (List *)mem_alloc(memory_manager, sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void free_list(memoryManagerADT memory_manager, List *list) {
    if (list == NULL) {
        return;
    }

    ListNode *current = list->head;
    ListNode *next;
    while (list->size > 0) {
        next = current->next;
        mem_free(memory_manager, current);
        current = next;
        list->size--;
    }
    mem_free(memory_manager, list);
}

uint8_t list_add(memoryManagerADT memory_manager, List *list, void *data) {
    if (list == NULL || data == NULL) {
        return 0;
    }
    ListNode *new_node = (ListNode *)mem_alloc(memory_manager, sizeof(ListNode));
    if (new_node == NULL) {
        return 0;
    }
    new_node->data = data;
    if (list->head == NULL) {
        list->head = new_node;
        list->last = new_node;
        new_node->next = new_node;
    } else {
        list->last->next = new_node;
        new_node->next = list->head;
    }
    list->size++;
    return 1;
}

uint8_t list_remove(memoryManagerADT memory_manager, List *list, void *data) {
    if (list == NULL || list->head == NULL || data == NULL) {
        return 0;
    }
    ListNode *current = list->head;
    ListNode *previous = NULL;
    while(current->data != data) {
        if (current == list->last) {
            return 0;
        }
        previous = current;
        current = current->next;
    }
    previous->next = current->next;
    mem_free(memory_manager, current);
    return 1;
}

size_t list_size(memoryManagerADT memory_manager, const List *list) {
    if (list == NULL) {
        return 0;
    }
    return list->size;
}

uint8_t list_is_empty(memoryManagerADT memory_manager, const List *list) {
    if (list == NULL) {
        return 1;
    }
    return list->size == 0;
}