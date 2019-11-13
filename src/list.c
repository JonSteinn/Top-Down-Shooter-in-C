#include "list.h"


static struct ListItem* __get_next_allocated(List* list);


List* init_list(int32_t memory, size_t data_size) {
    List* list = (List*)malloc(sizeof(List));
    list->data_size = data_size;
    list->mem_size = memory;
    list->index = 0;
    list->head = NULL;
    list->pre_allocated = (struct ListItem*)malloc(sizeof(struct ListItem) * list->mem_size);
    for (int32_t i = 0; i < list->mem_size; i++) {
        (list->pre_allocated + i)->data = (void*)malloc(data_size);
    }
    return list;
}

void list_add(List* list, void* data) {
    struct ListItem* tmp = __get_next_allocated(list);
    memcpy(tmp->data, data, list->data_size);
    if (list->head == NULL) {
        list->head = tmp;
    } else {
        tmp->next = list->head;
        list->head = tmp;
    }
}

void list_iterate(List* list, bool (*f)(void*)) {
    struct ListItem** it_ptr = &list->head;
    struct ListItem* it = list->head;
    while (it) {
        if (!f(it->data)) *it_ptr = it->next;
        it_ptr = &it->next;
        it = it->next;
    }
}

void destroy_list(List* list) {
    for (int32_t i = 0; i < list->mem_size; i++) {
        free((list->pre_allocated + i)->data);
    }
    free(list->pre_allocated);
    free(list);
}




static struct ListItem* __get_next_allocated(List* list) {
    struct ListItem* n = list->pre_allocated + list->index++;
    n->next = NULL;
    if (list->index == list->mem_size) list->index = 0;
    return n;
}