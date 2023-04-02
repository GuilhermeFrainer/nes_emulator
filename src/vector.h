#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
// Double instead
#define REALLOC_SIZE 10

// uint_8 vector to store a program
typedef struct Vector
{
    uint8_t *list;
    size_t capacity;
    size_t size;
} Vector;

Vector new_vector(uint32_t);
void append(Vector *vector, uint8_t new_item);
void resize_vector(Vector *vector);
void destroy(Vector *vector);

#endif