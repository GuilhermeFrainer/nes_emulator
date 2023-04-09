#include <stdio.h>
#include "instructions.h"
#include "cpu.h"

Instruction inst_list[0xFF];

void populate_inst_list(void)
{
    for (int i = 0x00; i < 0xFF + 1; i++)
    {
        inst_list[i] = get_instruction_from_opcode(i);
    }
}

Instruction get_instruction_from_opcode(uint8_t opcode)
{
    Instruction inst;
    switch (opcode)
    {
    // opcode, mnemonic, bytes, cycles, mode
    case 0x29: inst = (Instruction) {0x29, "AND", 2, 2, Immediate}; return inst;
    case 0x25: inst = (Instruction) {0x25, "AND", 2, 3, ZeroPage}; return inst;
    case 0x35: inst = (Instruction) {0x35, "AND", 2, 4, ZeroPageX}; return inst;
    case 0x2D: inst = (Instruction) {0x2D, "AND", 3, 4, Absolute}; return inst;
    case 0x3D: inst = (Instruction) {0x3D, "AND", 3, 4, AbsoluteX}; return inst;
    case 0x39: inst = (Instruction) {0x39, "AND", 3, 4, AbsoluteY}; return inst;
    case 0x21: inst = (Instruction) {0x21, "AND", 2, 6, IndirectX}; return inst;
    case 0x31: inst = (Instruction) {0x31, "AND", 2, 5, IndirectY}; return inst;

    case 0x0A: inst = (Instruction) {0x0A, "ASL", 1, 2, Implied}; return inst;
    case 0x06: inst = (Instruction) {0x06, "ASL", 2, 5, ZeroPage}; return inst;
    case 0x16: inst = (Instruction) {0x16, "ASL", 2, 6, ZeroPageX}; return inst;
    case 0x0E: inst = (Instruction) {0x0E, "ASL", 3, 6, Absolute}; return inst;
    case 0x1E: inst = (Instruction) {0x1E, "ASL", 3, 7, AbsoluteX}; return inst;

    case 0x90: inst = (Instruction) {0x90, "BCC", 2, 2, Implied}; return inst;

    case 0x00: inst = (Instruction) {0x00, "BRK", 1, 7, Implied}; return inst;

    case 0xE8: inst = (Instruction) {0xE8, "INX", 1, 2, Implied}; return inst;

    case 0xA9: inst = (Instruction) {0xA9, "LDA", 2, 2, Immediate}; return inst;
    case 0xA5: inst = (Instruction) {0xA5, "LDA", 2, 3, ZeroPage}; return inst;
    case 0xB5: inst = (Instruction) {0xB5, "LDA", 2, 4, ZeroPageX}; return inst;
    case 0xAD: inst = (Instruction) {0xAD, "LDA", 3, 4, Absolute}; return inst;
    case 0xBD: inst = (Instruction) {0xBD, "LDA", 3, 4, AbsoluteX}; return inst;
    case 0xB9: inst = (Instruction) {0xB9, "LDA", 3, 4, AbsoluteY}; return inst;
    case 0xA1: inst = (Instruction) {0xA1, "LDA", 2, 6, IndirectX}; return inst;
    case 0xB1: inst = (Instruction) {0xB1, "LDA", 2, 5, IndirectY}; return inst;

    case 0xAA: inst = (Instruction) {0xAA, "TAX", 1, 2, Implied}; return inst;

    default: inst = (Instruction) {0x00, "BRK", 1, 7, Implied}; return inst;
    }
}

// Instructions

void and(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    set_reg_a(cpu, cpu->reg_a & operand);
}

void asl_acc(CPU *cpu)
{
    update_carry_flag(cpu, cpu->reg_a);
    set_reg_a(cpu, cpu->reg_a << 1);
}

void asl(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t value = read_mem(cpu, addr);
    write_mem(cpu, value, addr);
    update_carry_flag(cpu, value);
}

uint8_t bcc(CPU *cpu)
{
    if ((cpu->status & CARRY_FLAG) != 0)
    {
        return read_mem(cpu, cpu->program_counter);    
    }
    else
    {
        return 0;
    }
}

void brk(void)
{
    return;
}

void inx(CPU *cpu)
{
    cpu->reg_x++;
    update_zero_and_negative_flags(cpu, cpu->reg_x);
}

void lda(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t value = read_mem(cpu, addr);
    set_reg_a(cpu, value);
}

void tax(CPU *cpu)
{
    set_reg_x(cpu, cpu->reg_a);
}

// Register functions
void set_reg_a(CPU *cpu, uint8_t value)
{
    cpu->reg_a = value;
    update_zero_and_negative_flags(cpu, value);
}

void set_reg_x(CPU *cpu, uint8_t value)
{
    cpu->reg_x = value;
    update_zero_and_negative_flags(cpu, value);
}

void set_reg_y(CPU *cpu, uint8_t value)
{
    cpu->reg_y = value;
    update_zero_and_negative_flags(cpu, value);
}

// Utility functions

void update_zero_and_negative_flags(CPU *cpu, uint8_t result)
{
    // Update zero flag
    if (result == 0)
    {
        cpu->status = cpu->status | ZERO_FLAG;
    }
    else
    {
        cpu->status = cpu->status & ~ZERO_FLAG;
    }
    // Update negative flag
    if ((result & 0b10000000) != 0)
    {
        cpu->status = cpu->status | NEGATIVE_FLAG;
    }
    else
    {
        cpu->status = cpu->status & ~NEGATIVE_FLAG;
    }
}

void update_carry_flag(CPU *cpu, uint8_t value)
{
    if ((value >> 7) == 1)
    {
        cpu->status = cpu->status | CARRY_FLAG;
    }
    else
    {
        cpu->status = cpu->status & ~CARRY_FLAG;
    }
}

uint16_t get_operand_addr(CPU *cpu, AddrMode mode)
{
    switch (mode)
    {
        case Immediate: return cpu->program_counter;

        case ZeroPage: return read_mem(cpu, cpu->program_counter);
        case ZeroPageX: return read_mem(cpu, cpu->program_counter) + cpu->reg_x;
        case ZeroPageY: return read_mem(cpu, cpu->program_counter) + cpu->reg_y;

        case Absolute: return read_mem_u16(cpu, cpu->program_counter);
        case AbsoluteX: return read_mem_u16(cpu, cpu->program_counter) + cpu->reg_x;
        case AbsoluteY: return read_mem_u16(cpu, cpu->program_counter) + cpu->reg_y;

        case Indirect:
            uint16_t base_u16 = read_mem_u16(cpu, cpu->program_counter);
            return read_mem_u16(cpu, base_u16);
        case IndirectX:
            uint8_t base_u8 = read_mem(cpu, cpu->program_counter);
            return read_mem_u16(cpu, base_u8 + cpu->reg_x);
        case IndirectY:
            base_u16 = read_mem_u16(cpu, cpu->program_counter);
            return read_mem_u16(cpu, base_u16) + cpu->reg_y;
    }
}