#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include <stdbool.h>

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

void populate_inst_list(void);
Instruction get_instruction_from_opcode(uint8_t opcode);

// Instructions
// In alphabetical order

void adc(CPU *cpu, AddrMode mode);
void and(CPU *cpu, AddrMode mode);
void asl_acc(CPU *cpu);
void asl(CPU *cpu, AddrMode mode);

// Branch instructions part 1
void bcc(CPU *cpu);
void bcs(CPU *cpu);
void beq(CPU *cpu);

void bit(CPU *cpu, AddrMode mode);

// Branch instructions part 2
void bmi(CPU *cpu);
void bne(CPU *cpu);
void bpl(CPU *cpu);

void brk(CPU *cpu);

// Branch instructions part 3
void bvc(CPU *cpu);
void bvs(CPU *cpu);

// Clear instructions
void clc(CPU *cpu);
void cld(CPU *cpu);
void cli(CPU *cpu);
void clv(CPU *cpu);

// Compare instructions
void cmp(CPU *cpu, AddrMode mode);
void cpx(CPU *cpu, AddrMode mode);
void cpy(CPU *cpu, AddrMode mode);

// Decrement instructions
void dec(CPU *cpu, AddrMode mode);
void dex(CPU *cpu);
void dey(CPU *cpu);

void eor(CPU *cpu, AddrMode mode);

// Increment instructions
void inc(CPU *cpu, AddrMode mode);
void inx(CPU *cpu);
void iny(CPU *cpu);

// Jump instructions
void jmp(CPU *cpu, AddrMode mode);
void jsr(CPU *cpu);

// Load instructions
void lda(CPU *cpu, AddrMode mode);
void ldx(CPU *cpu, AddrMode mode);
void ldy(CPU *cpu, AddrMode mode);

void lsr_acc(CPU *cpu);
void lsr(CPU *cpu, AddrMode mode);
//void nop(void); // Not implemented since it does nothing
void ora(CPU *cpu, AddrMode mode);

// Push instructions
void pha(CPU *cpu);
void php(CPU *cpu);

// Pull instructions
void pla(CPU *cpu);
void plp(CPU *cpu);

// Rotate instructions
void rol_acc(CPU *cpu);
void rol(CPU *cpu, AddrMode mode);
void ror_acc(CPU *cpu);
void ror(CPU *cpu, AddrMode mode);

// Return instructions
void rti(CPU *cpu);
void rts(CPU *cpu);

void sbc(CPU *cpu, AddrMode mode);

// Set flag instructions
void sec(CPU *cpu);
void sed(CPU *cpu);
void sei(CPU *cpu);

// Store register
void sta(CPU *cpu, AddrMode mode);
void stx(CPU *cpu, AddrMode mode);
void sty(CPU *cpu, AddrMode mode);

// Transfer instructions
void tax(CPU *cpu);
void tay(CPU *cpu);
void tsx(CPU *cpu);
void txa(CPU *cpu);
void txs(CPU *cpu);
void tya(CPU *cpu);

// End instructions

// Utility functions
void branch(CPU *cpu, bool condition);
void update_zero_and_negative_flags(CPU *cpu, uint8_t result);
void update_carry_flag(CPU *cpu, uint8_t value);
uint16_t get_operand_addr(CPU *cpu, AddrMode mode);
void add_with_carry(CPU *cpu, uint8_t operand);

#endif