#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

typedef struct CPU CPU;

typedef enum AddrMode
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

typedef struct Instruction
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
#define OVERFLOW_FLAG 0b01000000
#define ZERO_FLAG 0b00000010
#define CARRY_FLAG 0b00000001

void populate_inst_list(void);
Instruction get_instruction_from_opcode(uint8_t opcode);

// Instructions
void adc(CPU *cpu, AddrMode mode);
void and(CPU *cpu, AddrMode mode);
void asl_acc(CPU *cpu);
void asl(CPU *cpu, AddrMode mode);
uint8_t bcc(CPU *cpu);
uint8_t bcs(CPU *cpu);
void brk();
void inx(CPU *cpu);
void lda(CPU *cpu, AddrMode mode);
void tax(CPU *cpu);

// Register functions
void set_reg_a(CPU *cpu, uint8_t value);
void set_reg_x(CPU *cpu, uint8_t value);
void set_reg_y(CPU *cpu, uint8_t value);

// Utility functions
void update_zero_and_negative_flags(CPU *cpu, uint8_t result);
void update_carry_flag(CPU *cpu, uint8_t value);
uint16_t get_operand_addr(CPU *cpu, AddrMode mode);

#endif