#include <stdio.h>
#include "instructions.h"
#include "cpu.h"

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
        cpu->status = cpu->status & !ZERO_FLAG;
    }
    // Update negative flag
    if (result & 0b10000000 != 0)
    {
        cpu->status = cpu->status | NEGATIVE_FLAG;
    }
    else
    {
        cpu->status = cpu->status & !NEGATIVE_FLAG;
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
            uint16_t base = read_mem_u16(cpu, cpu->program_counter);
            return read_mem_u16(cpu, base);
        case IndirectX:
            uint8_t base = read_mem(cpu, cpu->program_counter);
            return read_mem_u16(cpu, base + cpu->reg_x);
        case IndirectY:
            uint16_t base = read_mem_u16(cpu, cpu->program_counter);
            return base + cpu->reg_y;
        case NoneAddressing: return;
    }
}