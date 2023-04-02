#include "cpu.h"

void inx(CPU *cpu)
{
    cpu->reg_x++;
}

void lda(CPU *cpu, uint8_t value)
{
    cpu->reg_a = value;
}

void tax(CPU *cpu)
{
    cpu->reg_x = cpu->reg_a;
}