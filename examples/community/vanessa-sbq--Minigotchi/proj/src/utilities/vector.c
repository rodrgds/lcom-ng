#include "vector.h"
#include <stdio.h>

Vector* init_vector(size_t initial_capacity) {

    Vector *vector =(Vector *) malloc(sizeof(*vector));

    if (vector == NULL) return NULL;

    vector->data = (uint8_t *)malloc(initial_capacity * sizeof(uint8_t));
    if (vector->data == NULL) {
        panic("Failed to allocate memory for vector\n");
        exit(EXIT_FAILURE);
    }
    vector->size = 0;
    vector->capacity = initial_capacity;
    return vector;
}

void resize_vector(Vector *vec, size_t new_capacity) {
    uint8_t *new_data = (uint8_t *)realloc(vec->data, new_capacity * sizeof(uint8_t));
    if (new_data == NULL) {
        panic("Failed to allocate memory for vector\n");
        free_vector(vec);
        exit(EXIT_FAILURE);
    }
    vec->data = new_data;
    vec->capacity = new_capacity;
}

void push_back(Vector *vec, uint8_t unsignedInt) {
    if (vec->size == vec->capacity) {
        resize_vector(vec, vec->capacity + 1);
    }
    vec->data[vec->size++] = unsignedInt;
}

void remove_at(Vector *vec, size_t index) {
    if (index >= vec->size) {
        printf("Index out of bounds\n");
        return;
    }
    for (size_t i = index; i < vec->size - 1; i++) {
        vec->data[i] = vec->data[i + 1];
    }
    vec->size--;
    if (vec->size > 0 && vec->size == vec->capacity / sizeof(uint8_t)) {
        resize_vector(vec, vec->capacity - 1);
    }
}

void free_vector(Vector *vec) {
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}
