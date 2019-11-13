#ifndef xcW0Voy9iu_LIST_H
#define xcW0Voy9iu_LIST_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/**
 * Struct:
 *  struct ListItem
 *
 * Purpose:
 *  A node in the list.
 *
 * Fields:
 *  - data:
 *      The data to store in the list.
 *  - next:
 *      The pointer to the next element.
 */
struct ListItem {
    void*               data;
    struct ListItem*    next;
};

/**
 * Struct:
 *  List
 *
 * Purpose:
 *  A singly linked list that pre-allocates memory for its nodes
 *  in a fixed size circular array. Suppose the array hase size
 *  X, then for any element added, it must be gone before another
 *  X-1 have been added. This limits the lifetime of each element
 *  and is to be used for short lived objects.
 *
 * Fields:
 *  - data_size:
 *      The number of bytes in the data being stored in each node.
 *  - mem_size:
 *      The amount of pre-allocated memory used for list.
 *  - index:
 *      The current index in the pre-allocated memory array.
 *  - pre_allocated:
 *      Array of pre-allocated nodes.
 *  - head:
 *      First element in the list.
 */
typedef struct {
    size_t              data_size;
    int32_t             mem_size;
    int32_t             index;
    struct ListItem*    pre_allocated;
    struct ListItem*    head;
} List;

/**
 * Function:
 *  init_list
 *
 * Purpose:
 *  Allocate all resources for the list, given a
 *  specific data type of all its nodes.
 *
 * Parameters:
 *  - memory:
 *      How much pre-allocated memory should be allocated.
 *  - data_size:
 *      What is the data size of the node data in bytes.
 *
 * Returns:
 *  A allocated and initialized list.
 */
List* init_list(int32_t memory, size_t data_size);

/**
 * Function:
 *  list_add
 *
 * Purpose:
 *  Prepends data to the list.
 *
 * Parameters:
 *  - data:
 *      The data to add to the list. The data is
 *      copied so the caller should handle cleaning
 *      up the resource of the parameter if needed.
 *
 * Returns:
 *  Nothing.
 */
void list_add(List* list, void* data);

/**
 * Function:
 *  list_iterate
 *
 * Purpose:
 *  Iterate through the list and perform a given function on
 *  every element. The provided funtion can return false to
 *  remove a specific element.
 *
 * Parameters:
 *  - list:
 *      The list to iterate through.
 *  - f:
 *      A function with data type as input (casted to a void pointer)
 *      that returns false if the input element should be removed
 *      from the list, true otherwise.
 *
 * Returns:
 *  Nothing.
 */
void list_iterate(List* list, bool (*f)(void*));

/**
 * Function:
 *  destroy_list
 *
 * Purpose:
 *  Free any resources in use by a list.
 *
 * Parameters:
 *  - list:
 *      The list to free.
 *
 * Returns:
 *  Nothing.
 */
void destroy_list(List* list);

#endif