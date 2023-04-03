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
        cpu->status = cpu->status | 0b00000010;
    }
    else
    {
        cpu->status = cpu->status & 0b11111101;
    }
    // Update negative flag
    if (result & 0b10000000 != 0)
    {
        cpu->status = cpu->status | 0b10000000;
    }
    else
    {
        cpu->status = cpu->status & 0b01111111;
    }
}