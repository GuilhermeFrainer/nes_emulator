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

    Implied,

    Relative
} AddrMode;

typedef struct Instruction
{
    uint8_t opcode;
    char mnemonic[5];
    int bytes;
    int cycles;
    AddrMode mode;
} Instruction;

extern Instruction inst_list[0x100];

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

// Unofficial opcodes

// AND+ instructions
void anc(CPU *cpu, AddrMode mode);
void arr(CPU *cpu, AddrMode mode);
void asr(CPU *cpu, AddrMode mode);

void dcp(CPU *cpu, AddrMode mode);

void isb(CPU *cpu, AddrMode mode);

void lae(CPU *cpu, AddrMode mode);

void lax(CPU *cpu, AddrMode mode);

void lxa(CPU *cpu, AddrMode mode);

// Rotate instructions
void rla(CPU *cpu, AddrMode mode);
void rra(CPU *cpu, AddrMode mode);

void sax(CPU *cpu, AddrMode mode);

void sbx(CPU *cpu, AddrMode mode);

void sha(CPU *cpu, AddrMode mode);

// AND with high byte instructions
void shs(CPU *cpu, AddrMode mode);
void shx(CPU *cpu, AddrMode mode);
void shy(CPU *cpu, AddrMode mode);

// Shift instructions
void slo(CPU *cpu, AddrMode mode);
void sre(CPU *cpu, AddrMode mode);

void xaa(CPU *cpu, AddrMode mode);

// End instructions

// Utility functions
void branch(CPU *cpu, bool condition);
void update_zero_and_negative_flags(CPU *cpu, uint8_t result);
void update_carry_flag(CPU *cpu, uint8_t value);
uint8_t get_operand(CPU *cpu, AddrMode mode);
uint16_t get_operand_addr(CPU *cpu, AddrMode mode);
void add_with_carry(CPU *cpu, uint8_t operand);
void deal_with_page_crossing(CPU *cpu, uint16_t base_addr, uint16_t end_addr);

#endif
