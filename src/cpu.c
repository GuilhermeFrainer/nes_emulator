#include "cpu.h"
#include "instructions.h"

CPU new_cpu(void)
{
    CPU cpu;
    cpu.status = 0;
    cpu.program_counter = 0;
    cpu.reg_a = 0;
    cpu.reg_x = 0;
    cpu.reg_y = 0;
    return cpu;
}

void interpret(CPU *cpu, uint8_t program[])
{
    while (1)
    {
        switch (program[cpu->program_counter])
        {
            case 0x00: brk(cpu); return;
            case 0xe8: inx(cpu); break;
            case 0xa9: lda(cpu, program[cpu->program_counter + 1]); break;
            case 0xaa: tax(cpu); break;
        }
        cpu->program_counter += 1;
    }
}