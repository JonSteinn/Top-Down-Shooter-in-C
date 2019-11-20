/**********************************************************
 * FROM:                                                  *
 *      https://github.com/JonSteinn/FastSinglyLinkedList *
 **********************************************************/

#ifndef __P4H95NNBPR_LIST_H__
#define __P4H95NNBPR_LIST_H__

#include <stdint.h>  // uint32_t, uint64_t
#include <stdlib.h>  // free, malloc and calloc
#include <stddef.h>  // NULL
#include <string.h>  // memcpy

/**
 * Enum:
 *  IterationAction
 *
 * Purpose:
 *  What should be done at each element, when iterating.
 *
 * Constants:
 *  - IT_REMOVE:
 *      The element should be removed.
 *  - IT_KEEP:
 *      The element should be kept.
 *  - IT_STOP:
 *      Stop iterating after current element.
 */
typedef enum {
    IT_REMOVE = 0,
    IT_KEEP   = 1,
    IT_STOP   = 2
} IterationAction;

/**
 * Struct:
 *  struct __ListItem__
 *
 * Purpose:
 *  A node in the list. Not to be used directly by
 *  the list user.
 *
 * Fields:
 *  - data:
 *      The data to store in the list.
 *  - next:
 *      A pointer to the next element.
 */
struct __ListItem__ {
    void*                   data;
    struct __ListItem__*    next;
};

/**
 * A pointer to a list element.
 */
typedef struct __ListItem__* Node;

/**
 * A pointer to a list element pointer.
 */
typedef Node* NodePtr;

/**
 * IterationAction fun(void* current_element, void* aditional_data) { ... }
 *
 * Additional data can be passed if the list should alter
 * data outside the list. If fun returns IT_REMOVE, we remove
 * the current element. If it returns IT_STOP, we stop iterating.
 * Otherwise we continue and keep the current element in the list.
 */
typedef IterationAction (*it_fun)(void*, void*);

/**
 * void fun(void* data) { ... }
 *
 * If the data stored in the list holds allocated data that
 * needs to be freed by the iterator, then it can be done
 * here.
 */
typedef void (*data_free_fun)(void*);

/**
 * Struct:
 *  List
 *
 * Purpose:
 *  Holds all internal related resources for the list.
 *
 * Fields:
 *  - queue_head:
 *      The head of the queue that holds nodes not in use.
 *  - queue_tail:
 *      The tail of the queue that holds nodes not in use.
 *  - head:
 *      The head of the list.
 *  - data_size:
 *      The size of the data stored in the list.
 *  - max_capacity:
 *      The maximum number of elements the list can carry.
 *  - data_free:
 *      An optional free function for data. It should not free
 *      the data object itself but rather any internal data that
 *      needs to be freed. This is called each time the queue feeds
 *      the list a node.
 *  - pre_allocated_nodes:
 *      A pointer to the pre-allocated nodes so it can be freed later.
 *  - pre_allocated_data:
 *      A pointer to the pre-allocated data so it can be freed later.
 */
typedef struct {
    Node            queue_head;
    Node            queue_tail;
    Node            head;
    uint64_t        data_size;
    uint64_t        max_capacity;
    data_free_fun   data_free;
    void*           pre_allocated_nodes;
    void*           pre_allocated_data;
} List;

/**
 * Function:
 *  list_init
 *
 * Purpose:
 *  Allocate memory and initialize a List object.
 *
 * Parameters:
 *  - max_capacity:
 *      How many elements the list should be able to carry at most.
 *  - data_size:
 *      The size of the data type stored in the list.
 *
 * Returns:
 *  A List.
 */
List* list_init(uint64_t max_capacity, uint64_t data_size);

/**
 * Function:
 *  list_init_full
 *
 * Purpose:
 *  Allocate memory and initialize a List object.
 *
 * Parameters:
 *  - max_capacity:
 *      How many elements the list should be able to carry at most.
 *  - data_size:
 *      The size of the data type stored in the list.
 *  - data_free:
 *      An internal free function for the data stored in the list.
 *
 * Returns:
 *  A List.
 */
List* list_init_full(uint64_t max_capacity, uint64_t data_size, data_free_fun data_free);

/**
 * Function:
 *  list_add
 *
 * Purpose:
 *  Add an element to the front of the list.
 *
 * Parameters:
 *  - list:
 *      The list to prepend to.
 *  - element:
 *      The element to add.
 *
 * Returns:
 *  Nothing.
 */
void list_add(List* list, void* element);


/**
 * Function:
 *  list_iterate
 *
 * Purpose:
 *  Iterate through all or some elements of the list and
 *  optionally remove them.
 *
 * Parameters:
 *  - list:
 *      The list to iterate through.
 *  - fun:
 *      A function to apply to each element. It should return
 *      IT_REMOVE if that element should be deleted, IT_STOP
 *      if we should stop the iteration and IT_KEEP to continue
 *      the iteration and keep the element in the list.
 *  - data:
 *      Additional data to pass to fun.
 *
 * Returns:
 *  Nothing.
 */
void list_iterate(List* list, it_fun fun, void* data);


/**
 * Function:
 *  list_destroy
 *
 * Purpose:
 *  Free all resources in use by the list.
 *
 * Parameters:
 *  - list:
 *      The list to destroy.
 *
 * Returns:
 *  Nothing.
 */
void list_destroy(List* list);

#endif