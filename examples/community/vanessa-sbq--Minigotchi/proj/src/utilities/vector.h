#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdio.h>
#include <stdlib.h>

#include <lcom/lcf.h>

#include "../viewer/guiDrawer.h"

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Vector;

Vector* init_vector(size_t initial_capacity);
void resize_vector(Vector *vec, size_t new_capacity);
void push_back(Vector *vec, uint8_t unsignedInt);
void remove_at(Vector *vec, size_t index);
void free_vector(Vector *vec);

#endif /* _VECTOR_H_ */
