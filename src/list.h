#ifndef xcW0Voy9iu_LIST_H
#define xcW0Voy9iu_LIST_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct ListItem {
    void*               data;
    struct ListItem*    next;
};

typedef struct ListItem ListItem;
typedef ListItem* Node;

typedef struct {
    size_t      data_size;
    int32_t     mem_size;
    int32_t     index;
    ListItem*   pre_allocated;
    Node        head;
} List;

List* init_list(int32_t memory, size_t data_size);
void list_add(List* list, void* data);
void list_iterate(List* list, bool (*f)(void*));
void destroy_list(List* list);

#endif