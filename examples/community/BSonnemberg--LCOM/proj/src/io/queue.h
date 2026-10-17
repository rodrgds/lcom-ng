#include <lcom/lcf.h>

typedef struct node {
    int value;
    int size;
    struct node* next;
}

typedef struct queue {
    node* front;
    node* back;
}

int enqueue(queue* q, int val);
int dequeue(queue* q, int* val);
int getSize(queue* q);