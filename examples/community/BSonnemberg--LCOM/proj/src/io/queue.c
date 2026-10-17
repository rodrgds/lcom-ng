#include "queue.h"

void init(struct queue* q) {
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
}

int enqueue(queue* q, int val) {
    if(q == NULL) return 1;
    node* node = malloc(sizeof(node));
    node->value = val;
    node->next = NULL;
    q->size = q->size + 1;

    if(q->back != NULL) {
        q->back->next = node;
    }

    q->back = node;
    if(q->front == NULL) q->front = node;

    return 0;
}


int dequeue(queue* q, int* val) {
    if(q->front == NULL) return 1;

    *val = q->front->val;
    q->front = q->front->next;
    q->size = q->size - 1;
    if(q->front == NULL) q->back = NULL;
    return 0;
}

int getSize(queue* q) {
    return q->size;
}