#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"
#include <stdint.h>

typedef enum
{
    Immediate,

    ZeroPage,
    ZeroPageX,
    ZeroPageY,

    Absolute,
    AbsoluteX,
    AbsoluteY,

    Indirect,
    IndirectX,
    IndirectY,

    Implied
} AddrMode;

typedef struct
{
    uint8_t opcode;
    char mnemonic[3];
    int bytes;
    int cycles;
    AddrMode mode;
} Instruction;

extern Instruction inst_list[0xFF];

// CPU flag bits
#define NEGATIVE_FLAG 0b10000000
#define ZERO_FLAG 0b00000010

void populate_inst_list(void);
Instruction get_instruction_from_opcode(uint8_t opcode);

void brk();
void inx(CPU *cpu);
void lda(CPU *cpu, AddrMode mode);
void tax(CPU *cpu);
void update_zero_and_negative_flags(CPU *cpu, uint8_t result);
uint16_t get_operand_addr(CPU *cpu, AddrMode mode);

#endif