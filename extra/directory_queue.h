#ifndef SKETCH_CORE_DIRECTORY_QUEUE_H
#define SKETCH_CORE_DIRECTORY_QUEUE_H

typedef struct {
    char* p_path;
    int p_is_dir;
} item;

/**
 * Add a new item to the queue.
 * @param queue The queue.
 * @param new_item The new item.
 * @return SUCCESS if item is added successfully or -1.
 */
extern int add(item **queue, item* new_item);

/**
 * Take a peek at the first item of the queue.
 * @param queue The queue.
 * @return The pointer of the item.
 */
extern item* peek(item **queue);

/**
 * Pops the first item from the queue.
 * @param queue The queue.
 * @return The pointer of the item that popped.
 */
extern item* pop(item **queue);

#endif //SKETCH_CORE_DIRECTORY_QUEUE_H
