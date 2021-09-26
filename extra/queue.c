#include <include/queue.h>
#include <malloc.h>
#include <memory.h>
#include <include/codes.h>

queue *create_empty_queue() {
    return calloc(1, sizeof(queue));
}

int add(queue* c_queue, void *item) {
    if (c_queue == NULL || item == NULL) return -1;

    // Make the new c_queue to hold the new item.
    queue_node *new_node = calloc(1, sizeof(queue_node));
    new_node->q_item = item;
    new_node->q_next_node = NULL;

    if(c_queue->size == 0) {
        // Init start node
        c_queue->q_first_node = new_node;

        // Init end node
        c_queue->q_last_node = new_node;
        c_queue->q_last_node->q_next_node = new_node;

        // Increase size
        ++c_queue->size;

        return SUCCESS;
    }

    // Rewrite last node
    c_queue->q_last_node->q_next_node = new_node;
    c_queue->q_last_node = new_node;

    // Change size
    ++c_queue->size;

    return SUCCESS;
}

void *peek(queue *c_queue) {
    return c_queue->q_first_node->q_item;
}

void *pop(queue *c_queue) {
    void *removed_item = c_queue->q_first_node->q_item;

    if (removed_item == NULL || c_queue->size == 0) return NULL;

    --c_queue->size;

    queue_node  *tmp_node = c_queue->q_first_node;
    c_queue->q_first_node = c_queue->q_first_node->q_next_node;

    free(tmp_node);

    return removed_item;
}
