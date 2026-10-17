#include "queue.h"

/**
 * @brief Function to create a new node
 */
Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}


/**
 * @brief Function to initialize the queue
 */
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        return NULL;
    }
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}


/**
 * @brief Function to check if the queue is empty
 */
int isEmpty(Queue* queue) {
    return queue->size == 0;
}


/**
 * @brief Function to get the size of the queue
 */
int getSize(Queue* queue) {
    return queue->size;
}


/**
 * @brief Function to add an element to the queue (enqueue)
 */
void enqueue(Queue* queue, char data) {
    Node* newNode = createNode(data);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->size++;
}


/**
 * @brief Function to remove an element from the queue (dequeue)
 */
char dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return '\0';  // Return a null character if the queue is empty
    }
    Node* temp = queue->front;
    char data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}


/**
 * @brief Function to peek at the front element of the queue without removing it
 */
char peek(Queue* queue) {
    if (isEmpty(queue)) {
        return '\0';
    }
    return queue->front->data;
}
