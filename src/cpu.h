#ifndef CPU_H
#define CPU_H
#include <stdint.h>

typedef struct
{
    uint8_t status;
    uint16_t program_counter;
    uint8_t reg_a;
    uint8_t reg_x;
    uint8_t reg_y;
} CPU;

/* 
    STATUS REGISTER BITS
    NVss DIZC
    Negative, overflow, ss -> no effect
    Decimal, Interrupt disable, Zero, Carry
    Source: https://www.nesdev.org/wiki/Status_flags
*/

CPU new_cpu(void);
void interpret(CPU *cpu, uint8_t program[]);

#endif