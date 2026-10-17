#include <lcom/lcf.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define ERR_MAX_CAPACITY_EXCEEDED 1;

/**
 * @brief Data Structure for a Single Node
 */
typedef struct queue_node {
    void *value;
    struct queue_node *next;
} queue_node;


/**
 * @brief Data Structure for a Queue
 */
typedef struct queue {
    size_t size;
    size_t capacity;

    size_t base_size;

    queue_node *front;
    queue_node *back;
} queue;

/**
 * @brief Retrieves a pointer to the front/head element
 */
void *front(queue *q);

/**
 * @brief Retrieves a pointer to the the back/tail element
 */
void *back(queue *q);

/**
 * @brief Retrieves the size of the queue
 */
unsigned size(queue * q);


/**
 * @brief Returns true if the queue is empty and false otherwise
 */
bool is_empty(queue *q);

/**
 * @brief Returns true if the queue is full and false otherwise
 */
bool is_full(queue *q);

/**
 * @brief Creates a Queue
 * 
 * @param capacity Capacity of the queue
 * @param base_size Size, in bytes, of the elements of the queue
 * @return Queue* Pointer to the queue
 */
queue *queue_create(size_t capacity, size_t base_size);

/**
 * @brief Adds an element to the back of the queue 
 * 
 * @param queue Pointer to the queue
 * @param val Value to be enqueued
 * @return int Returns 0 if unsucessful
 */
int queue_push(queue *q, void *val);

/**
 * @brief Removes an element from the front of the queue
 * 
 * @param queue Pointer to the queue
 * @return void* Pointer to the element removed
 */
void *queue_pop(queue *q);

/**
 * @brief Destroys the queue
 * 
 * @note Everytime a Queue is used, at the end or in case of
 * errors that might end the program, this function should be
 * called, otherwise the memory allocated for the queue and
 * its nodes won't be freed.
 * 
 * @param queue Pointer to the queue
 */
void queue_destroy(queue *q);
