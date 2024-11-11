// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <utils/list.h>

typedef struct ListNode ListNode;
struct ListNode {
	void *data;
	ListNode *next;
};

struct List {
	ListNode *head;
	ListNode *next;
	ListNode *last;
	size_t size;
	memoryManagerADT memory_manager;
};

ListADT list_init(memoryManagerADT memory_manager) {
	ListADT list = (ListADT)mem_alloc(memory_manager, sizeof(struct List));
	if (list == NULL) return NULL;

	list->head = NULL;
	list->next = NULL;
	list->last = NULL;
	list->size = 0;
	list->memory_manager = memory_manager;
	return list;
}

void free_list(ListADT list) {
	if (list == NULL) return;

	ListNode *current = list->head;
	ListNode *next;
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

	ListNode *new_node = (ListNode *)mem_alloc(list->memory_manager, sizeof(ListNode));
	if (new_node == NULL) return 0;

	new_node->data = data;
	if (list_is_empty(list)) {
		list->head = new_node;
		list->next = new_node;
		list->last = new_node;
		new_node->next = new_node;
	} else {
		list->last->next = new_node;
		list->last = new_node;
		new_node->next = list->head;
	}
	list->size++;
	return 1;
}

int list_remove(ListADT list, void *data) {
	if (list == NULL || list_is_empty(list) || data == NULL) return 0;

	ListNode *current = list->head;
	ListNode *previous = list->last;
	while (current->data != data) {
		if (current == list->last) return 0;
		previous = current;
		current = current->next;
	}
	previous->next = current->next;
	if (current == list->head) list->head = current->next;
	if (current == list->next) list->next = current->next;
	if (current == list->last) list->last = previous;
	mem_free(list->memory_manager, current);
	list->size--;
	if (list->size == 0) {
		list->head = NULL;
		list->next = NULL;
		list->last = NULL;
	}
	return 1;
}

void *list_next(const ListADT list) {
	if (list == NULL || list_is_empty(list)) return NULL;
	void *data = list->next->data;
	list->next = list->next->next;
	return data;
}

size_t list_size(const ListADT list) {
	if (list == NULL) return 0;
	return list->size;
}

int list_is_empty(const ListADT list) {
	if (list == NULL) return 1;
	return list->size == 0;
}

void *get_element(const ListADT list, void *data) {
	if (list == NULL || list_is_empty(list) || data == NULL) return NULL;

	ListNode *current = list->head;
	while (current->data != data) {
		if (current == list->last) return NULL;
		current = current->next;
	}
	return current->data;
}
