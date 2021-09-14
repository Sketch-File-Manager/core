#ifndef SKETCH_CORE_QUEUE_H
#define SKETCH_CORE_QUEUE_H

typedef struct queue_node {
    void               *q_item;
    struct queue_node  *q_next_node;
} queue_node;

typedef struct queue {
    queue_node  *q_first_node;
    queue_node  *q_last_node;
    int          size;
} queue;


extern queue *create_empty_queue();

/**
 * Add a new item to the queue_node.
 * @param c_queue The queue_node.
 * @param new_item The new item.
 * @return SUCCESS if item is added successfully or -1.
 */
extern int add(queue* c_queue, void *item);

/**
 * Take a peek at the first item of the queue_node.
 * @param c_queue The queue_node.
 * @return The pointer of the item.
 */
extern void* peek(queue c_queue);

/**
 * Pops the first item from the queue_node.
 * @param c_queue The queue_node.
 * @return The pointer of the item that popped.
 */
extern void* pop(queue *c_queue);

#endif