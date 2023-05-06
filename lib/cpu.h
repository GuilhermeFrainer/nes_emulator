#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct Instruction Instruction;
typedef struct Bus Bus;
typedef struct ROM ROM;

typedef struct CPU
{
    uint8_t status;
    uint16_t program_counter;
    uint8_t stack_pointer;
    uint8_t reg_a;
    uint8_t reg_x;
    uint8_t reg_y;
    Bus *bus;
} CPU;

/* 
    STATUS REGISTER BITS
    7  bit  0
    ---- ----
    NVss DIZC
    |||| ||||
    |||| |||+- Carry
    |||| ||+-- Zero
    |||| |+--- Interrupt Disable
    |||| +---- Decimal
    ||++------ No CPU effect, see: the B flag
    |+-------- Overflow
    +--------- Negative
    Source: https://www.nesdev.org/wiki/Status_flags
*/

// CPU flag bits
#define NEGATIVE_FLAG  0b10000000
#define OVERFLOW_FLAG  0b01000000
#define BREAK_FLAG_1   0b00100000
#define BREAK_FLAG_0   0b00010000
#define DECIMAL_FLAG   0b00001000
#define INTERRUPT_FLAG 0b00000100
#define ZERO_FLAG      0b00000010
#define CARRY_FLAG     0b00000001

#define STACK 0x0100
#define STACK_RESET 0xFD

#define PROGRAM_START 0x0600
#define PROGRAM_START_ADDR 0x0800

CPU *new_cpu(ROM *rom);
void free_cpu(CPU *cpu);

// Memory functions
uint8_t mem_read(CPU *cpu, uint16_t addr);
uint16_t mem_read_u16(CPU *cpu, uint16_t addr);
void mem_write(CPU *cpu, uint8_t value, uint16_t addr);
void mem_write_u16(CPU *cpu, uint16_t value, uint16_t addr);

// Running functions
void reset(CPU *cpu);
void load(CPU *cpu, uint8_t program[], int program_length);
void run(CPU *cpu, SDL_Renderer *renderer, SDL_Texture *texture);
void interpret(CPU *cpu, uint8_t opcode);

// Register functions
void set_reg_a(CPU *cpu, uint8_t value);
void set_reg_x(CPU *cpu, uint8_t value);
void set_reg_y(CPU *cpu, uint8_t value);

// Flag functions
void set_flag(CPU *cpu, uint8_t flag);
void unset_flag(CPU *cpu, uint8_t flag);
bool is_set(CPU *cpu, uint8_t flag);

// Stack functions
uint16_t get_stack_addr(CPU *cpu);
void stack_push(CPU *cpu, uint8_t value);
void stack_push_u16(CPU *cpu, uint16_t value);
uint8_t stack_pull(CPU *cpu);
uint16_t stack_pull_u16(CPU *cpu);

#endif