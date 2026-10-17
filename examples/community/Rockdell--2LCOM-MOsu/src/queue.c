#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "circle.h"

queue q_new() {
	queue q = malloc(sizeof(queue_t));
	q->head = 0;
	q->tail = 0;
	q->alloc = 10;
	q->buf = malloc(sizeof(DATA) * (q->alloc));
	return q;
}

int empty(queue q) {
	return q->tail == q->head;
}

void enqueue(queue q, DATA n) {
	if(q->tail >= q->alloc)
		q->tail = 0;

	q->buf[q->tail++] = n;

	if(q->tail == q->alloc) {
		q->buf = realloc(q->buf, sizeof(DATA) * q->alloc * 2);

		if(q->head) {
			memcpy(q->buf +  q->head + q->alloc, q->buf + q->head, sizeof(DATA) * (q->alloc - q->head));
			q->head += q->alloc;
		}
		else
			q->tail = q->alloc;

		q->alloc *= 2;
	}
}

int dequeue(queue q, DATA *n) {
	if(q->head == q->tail)
		return 0;

	*n = q->buf[q->head++];

	if(q->head >= q->alloc) {
		q->head = 0;

		if(q->alloc >= 512 && q->tail < q->alloc / 2)
			q->buf = realloc(q->buf, sizeof(DATA) * (q->alloc /= 2));
	}

	return 1;
}
