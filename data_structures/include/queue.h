/* queue.h */
#ifndef SKETCH_CORE_QUEUE_H
#define SKETCH_CORE_QUEUE_H

typedef struct queue_node {
    void              *q_item;
    struct queue_node *q_next_node;
} queue_node;

typedef struct queue {
    queue_node *q_first_node;
    queue_node *q_last_node;
    int                q_size;
} queue;

/**
 * Returns an empty queue.
 * @return
 */
extern queue *create_empty_queue();

/**
 * Takes a peek a the first time of the queue.
 * @param c_queue The queue.
 * @return The item's data.
 */

static inline void *peek(queue *c_queue) {
    return c_queue->q_first_node->q_item;
}

/**
 * Add a new item to the queue_node.
 * @param c_queue The queue_node.
 * @param new_item The new item.
 * @return SUCCESS if item is added successfully or -1.
 */
extern int add(queue *c_queue, void *item);

/**
 * Pops the first item from the queue_node.
 * @param c_queue The queue_node.
 * @return The pointer of the item that popped.
 */
extern void *pop(queue *c_queue);

#endif