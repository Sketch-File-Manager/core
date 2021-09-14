#include <queue.h>
#include <malloc.h>
#include <memory.h>
#include <codes.h>

queue *create_empty_queue() {
    return calloc(1, sizeof(queue));
}

int add(queue* c_queue, void *item) {
    if (c_queue == NULL || item == NULL) return -1;

    // Make the new c_queue to hold the new item.
    queue_node *new_node = calloc(1, sizeof(queue_node));
    new_node->q_item = item;
    new_node->q_next_node = NULL;

    if (c_queue->size == 1 && c_queue->q_first_node->q_item != NULL) {
        c_queue->q_first_node = new_node;
        return SUCCESS;
    }

    // Connect the new c_queue to the last c_queue.
    ++c_queue->size;
    c_queue->q_last_node->q_next_node = new_node;
    c_queue->q_last_node = new_node;

    return SUCCESS;
}

void* peek(queue c_queue) {
    return c_queue.q_first_node->q_item;
}

/**
 * Pops the first item from the queue_node.
 * @param c_queue The queue_node.
 * @return The pointer of the item that popped.
 */
void* pop(queue *c_queue) {
    void *removed_item = c_queue->q_first_node->q_item;

    if (removed_item == NULL || c_queue->size == 0) return NULL;

    --c_queue->size;

    c_queue->q_first_node = c_queue->q_first_node->q_next_node;
    free(c_queue->q_first_node);
    return removed_item;
}
