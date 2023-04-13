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

// Store accumulator
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

// Register functions
void set_reg_a(CPU *cpu, uint8_t value);
void set_reg_x(CPU *cpu, uint8_t value);
void set_reg_y(CPU *cpu, uint8_t value);

// End instructions

// Utility functions
void update_zero_and_negative_flags(CPU *cpu, uint8_t result);
void update_carry_flag(CPU *cpu, uint8_t value);
uint16_t get_operand_addr(CPU *cpu, AddrMode mode);
void add_with_carry(CPU *cpu, uint8_t operand);
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