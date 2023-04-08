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
    uint8_t memory[0xFFFF];
} CPU;

/* 
    STATUS REGISTER BITS
    NVss DIZC
    Negative, overflow, ss -> no effect
    Decimal, Interrupt disable, Zero, Carry
    Source: https://www.nesdev.org/wiki/Status_flags
*/

CPU new_cpu(void);
uint8_t read_mem(CPU *cpu, uint16_t addr);
uint16_t read_mem_u16(CPU *cpu, uint16_t addr);
void write_mem(CPU *cpu, uint8_t value, uint16_t addr);
void write_mem_u16(CPU *cpu, uint16_t value, uint16_t addr);
void reset(CPU *cpu);
void load(CPU *cpu, uint8_t program[], int program_length);
void run(CPU *cpu);

#endif