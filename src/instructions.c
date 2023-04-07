#include <stdio.h>
#include "instructions.h"
#include "cpu.h"

Instruction get_instruction_from_opcode(uint8_t opcode)
{
    Instruction inst;
    switch (opcode)
    {
    // opcode, mnemonic, bytes, cycles, mode             
    case 0x00: inst = (Instruction) { 0x00, "BRK", 1, 7, Implied }; return inst;

    case 0xE8: inst = (Instruction) { 0xE8, "INX", 1, 2, Implied }; return inst;

    case 0xA9: inst = (Instruction) { 0xA9, "LDA", 2, 2, Immediate }; return inst;
    case 0xA5: inst = (Instruction) { 0xA5, "LDA", 2, 3, ZeroPage }; return inst;
    case 0xB5: inst = (Instruction) { 0xB5, "LDA", 2, 4, ZeroPageX }; return inst;
    case 0xAD: inst = (Instruction) { 0xAD, "LDA", 3, 4, Absolute }; return inst;
    case 0xBD: inst = (Instruction) { 0xBD, "LDA", 3, 4 /*+ 1 if page crossed*/, AbsoluteX }; return inst;
    case 0xB9: inst = (Instruction) { 0xB9, "LDA", 3, 4 /*+ 1 if page crossed*/, AbsoluteY }; return inst;
    case 0xA1: inst = (Instruction) { 0xA1, "LDA", 2, 6, IndirectX }; return inst;
    case 0xB1: inst = (Instruction) { 0xB1, "LDA", 2, 5 /*+ 1 if page crossed*/, IndirectY }; return inst;

    case 0xAA: inst = (Instruction) { 0xAA, "TAX", 1, 2, Implied }; return inst;
    }
}

void brk(CPU *cpu)
{
    return;
}

void inx(CPU *cpu)
{
    cpu->reg_x++;
    update_zero_and_negative_flags(cpu, cpu->reg_x);
}

void lda(CPU *cpu, uint8_t value)
{
    cpu->program_counter++;
    cpu->reg_a = value;
    update_zero_and_negative_flags(cpu, cpu->reg_a);
}

void tax(CPU *cpu)
{
    cpu->reg_x = cpu->reg_a;
    update_zero_and_negative_flags(cpu, cpu->reg_x);
}

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

uint16_t get_operand_addr(CPU *cpu, AddrMode mode)
{
    switch (mode)
    {
        case Immediate: return cpu->program_counter;

        case ZeroPage: return read_mem(cpu, cpu->program_counter);
        case ZeroPageX: return read_mem(cpu, cpu->program_counter + cpu->reg_x);
        case ZeroPageY: return read_mem(cpu, cpu->program_counter + cpu->reg_y);

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
            return base_u16 + cpu->reg_y;
    }
}