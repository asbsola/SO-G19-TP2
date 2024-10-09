#include <utils/list.h>

struct ListNode {
    void * data;              
    ListNode *next;
};
typedef struct ListNode ListNode;

struct List {
    ListNode *head;
    ListNode *last;
    size_t size;
    memoryManagerADT memory_manager;
};

ListADT list_init(memoryManagerADT memory_manager) {
    ListADT list = (ListADT) mem_alloc(memory_manager, sizeof(struct List));
    if (list == NULL) return NULL;

    list->head = NULL;
    list->last = NULL;
    list->size = 0;
    list->memory_manager = memory_manager;
    return list;
}

void free_list(ListADT list) {
    if (list == NULL) return;

    ListNode* current = list->head;
    ListNode* next;
    while (list->size > 0) {
        next = current->next;
        mem_free(list->memory_manager, current);
        current = next;
        list->size--;
    }
    mem_free(list->memory_manager, list);
}

int list_add(ListADT list, void *data) {
    if (list == NULL || data == NULL) return 0;

    ListNode* new_node = (ListNode*) mem_alloc(list->memory_manager, sizeof(ListNode));
    if (new_node == NULL) return 0;

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

int list_remove(ListADT list, void *data) {
    if (list == NULL || list_is_empty(list) || data == NULL)
        return 0;

    ListNode* current = list->head;
    ListNode* previous = list->last;
    while(current->data != data) {
        if (current == list->last) return 0;
        previous = current;
        current = current->next;
    }
    previous->next = current->next;
    mem_free(list->memory_manager, current);
    return 1;
}

size_t list_size(const ListADT list) {
    if (list == NULL) return 0;
    return list->size;
}

int list_is_empty(const ListADT list) {
    if (list == NULL) return 1;
    return list->size == 0;
}