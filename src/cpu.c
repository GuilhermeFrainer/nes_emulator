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

uint8_t read_mem(CPU *cpu, uint16_t addr)
{
    return cpu->memory[addr];
}

// Deals with NES's little-endianess
uint16_t read_mem_u16(CPU *cpu, uint16_t addr)
{
    uint16_t low = read_mem(cpu, addr);
    uint16_t high = read_mem(cpu, addr + 1);
    high = high << 8;
    return high | low;
}

void write_mem(CPU *cpu, uint8_t value, uint16_t addr)
{
    cpu->memory[addr] = value;
}

void write_mem_u16(CPU *cpu, uint16_t value, uint16_t addr)
{
    uint8_t low = value & 0xFF;
    uint8_t high = value >> 8;
    write_mem(cpu, low, addr);
    write_mem(cpu, high, addr + 1);
}

void reset(CPU *cpu)
{
    cpu->reg_a = 0;
    cpu->reg_x = 0;
    cpu->reg_y;
    cpu->status = 0;
    cpu->program_counter = read_mem_u16(cpu, 0xFFFC);
}

void load(CPU *cpu, uint8_t program[], int program_length)
{
    for (int i = 0; i < program_length; i++)
    {
        cpu->memory[i + 0x8000] = program[i];
    }
    write_mem_u16(cpu, 0x8000, 0xFFFC);
}