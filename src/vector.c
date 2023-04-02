#include "vector.h"

Vector new_vector(uint32_t size) 
{
    Vector vector;
    uint8_t* data = (uint8_t *) malloc(sizeof(uint8_t) * size);

    if (data == NULL)
    {
        exit(1);
    }
    vector.size = 0;
    vector.list = data;
    vector.capacity = 10;
    return vector; 
}

void append(Vector *vector, uint8_t new_item)
{
    if (vector->size >= vector->capacity)
    {
        resize_vector(vector);
    }
    vector->list[vector->size] = new_item;
    vector->size++;
}

void resize_vector(Vector *vector)
{
    uint8_t *new_list = (uint8_t *) realloc(vector->list, sizeof(uint8_t) * (vector->capacity + REALLOC_SIZE));
    if (new_list == NULL)
    {
        printf("realloc failed");
        exit(1);
    }
    vector->list = new_list;
    vector->capacity += REALLOC_SIZE;
}

void destroy(Vector *vector)
{
    free(vector->list);
}