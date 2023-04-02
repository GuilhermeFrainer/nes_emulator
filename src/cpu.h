#ifndef CPU_H
#define CPU_H
#include <stdint.h>

typedef struct CPU
{
    uint8_t reg_a;
    uint8_t status;
    uint16_t program_counter;
    uint8_t reg_x
} CPU;

#endif