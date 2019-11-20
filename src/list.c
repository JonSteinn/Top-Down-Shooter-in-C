#include "list.h"

/**
 * Function:
 *  __list_allocator
 *
 * Purpose:
 *  Allocate memory for the list it self and set some
 *  of its fields.
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
 *  A List object.
 */
static List* __list_allocator(uint64_t max_capacity, uint64_t data_size, data_free_fun data_free);

/**
 * Function:
 *  __node_allocator
 *
 * Purpose:
 *  Allocate memory for nodes and their data.
 *
 * Parameters:
 *  - list:
 *      The list holding the pre-allocated memory.
 *
 * Returns:
 *  Nothing.
 */
static void __node_allocator(List* list);

/**
 * Function:
 *  __init_queue
 *
 * Purpose:
 *  Construct a queue from the pre-allocated memory.
 *
 * Parameters:
 *  - list:
 *      The list holding the queue.
 *
 * Returns:
 *  Nothing.
 */
static void __init_queue(List* list);

/**
 * Function:
 *  __dequeue
 *
 * Purpose:
 *  Remove and return a node from the list.
 *
 * Parameters:
 *  - list:
 *      The list holding the queue.
 *
 * Returns:
 *  A node ready to be used by the list.
 */
static Node __dequeue(List* list);

/**
 * Function:
 *  __enqueue
 *
 * Purpose:
 *  Add a node back to the queue.
 *
 * Parameters:
 *  - list:
 *      The list holding the queue.
 *  - n:
 *      The node to add.
 *
 * Returns:
 *  Nothing.
 */
static void __enqueue(List* list, Node n);


/**
 * Calls list_init_full with data_free as NULL.
 */
List* list_init(uint64_t max_capacity, uint64_t data_size) {
    return list_init_full(max_capacity, data_size, NULL);
}

/**
 * Allocates memory for the list, nodes and data and then fills
 * a queue of all nodes, ready to be supplied to the list.
 */
List* list_init_full(uint64_t max_capacity, uint64_t data_size, data_free_fun data_free) {
    List* list = __list_allocator(max_capacity, data_size, data_free);
    __node_allocator(list);
    __init_queue(list);
    return list;
}

/**
 * Fetch a node from the queue and copy element's memory to
 * the node's data. We do not check if queue is empty, it is
 * the caller's responsibility to not go passed his limit.
 */
void list_add(List* list, void* element) {
    Node n = __dequeue(list);
    memcpy(n->data, element, list->data_size);
    n->next = list->head;
    list->head = n;
}

/**
 * Loop through the list and apply the function fun to
 * each element of the list (and also pass data to fun,
 * if the user needs to modify outside resources). If
 * fun returns IT_REMOVE, we remove the node from the
 * list and put it back in the queue. If fun returns
 * IT_STOP, the iteration stops.
 */
void list_iterate(List* list, it_fun fun, void* data) {
    IterationAction action = IT_KEEP;
    Node it = list->head;
    NodePtr it_ptr = &list->head;
    while (it && action != IT_STOP) {
        Node tmp = it->next;
        if (!(action=fun(it->data, data))) {
            *it_ptr = it->next;
            __enqueue(list, it);
        } else {
            it_ptr = &it->next;
        }
        it = tmp;
    }
}

/**
 * If custom free for data, then free those first, then
 * we free the pre-allocated data.
 */
void list_destroy(List* list) {
    if (list->data_free) {
        for (uint32_t i = 0; i < list->max_capacity; i++) {
            list->data_free(list->pre_allocated_data + i * list->data_size);
        }
    }
    free(list->pre_allocated_data);
    free(list->pre_allocated_nodes);
    free(list);
}

/**
 * Allocate memory for a list and set all fields.
 */
static List* __list_allocator(uint64_t max_capacity, uint64_t data_size, data_free_fun data_free) {
    List* list = (List*)malloc(sizeof(List));

    list->data_size             = data_size;
    list->max_capacity          = max_capacity;
    list->data_free             = data_free;
    list->head                  = NULL;
    list->queue_head            = NULL;
    list->queue_tail            = NULL;
    list->pre_allocated_data    = NULL;
    list->pre_allocated_nodes   = NULL;

    return list;
}

/**
 * The memory allocation for all nodes and their data.
 */
static void __node_allocator(List* list) {
    list->pre_allocated_nodes = (void*)malloc(sizeof(struct __ListItem__) * list->max_capacity);
    list->pre_allocated_data = (void*)calloc(list->max_capacity, list->data_size);
}

/**
 * Fill queue with the pre-allocated data.
 */
static void __init_queue(List* list) {
    for (uint32_t i = 0; i < list->max_capacity; i++) {
        Node tmp = (Node)(list->pre_allocated_nodes + sizeof(struct __ListItem__) * i);
        tmp->data = (list->pre_allocated_data + list->data_size * i);
        tmp->next = list->queue_head;
        list->queue_head = tmp;
    }
    list->queue_tail = (Node)list->pre_allocated_nodes;
}

/**
 * If the user initialized the list with a custom free,
 * then that is called here on the node's data. Next
 * link of the node is nullified here. There is no guard
 * against an empty queue and the user should not add
 * elements beyond the maximum capacity.
 */
static Node __dequeue(List* list) {
    Node n = list->queue_head;
    list->queue_head = list->queue_head->next;
    n->next = NULL;
    if (list->data_free) list->data_free(n->data);
    return n;
}

/**
 * Adds a node from the list, back to the queue.
 * The two scenarios are an empty queue (when
 * the list is full before a remove happens) and a
 * non-empty one.
 */
static void __enqueue(List* list, Node n) {
    n->next = NULL;
    if (list->queue_head == NULL) {
        list->queue_head = n;
    } else {
        list->queue_tail->next = n;
    }
    list->queue_tail = n;
}