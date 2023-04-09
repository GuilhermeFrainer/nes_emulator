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
    cpu->reg_y = 0;
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
    cpu->program_counter = 0x8000;
}

void run(CPU *cpu)
{
    while (1)
    {
        uint8_t opcode = read_mem(cpu, cpu->program_counter);
        Instruction inst = inst_list[opcode];
        uint16_t original_pc_state = cpu->program_counter;
        cpu->program_counter++;

        switch (opcode)
        {
        case 0x29:
        case 0x25:
        case 0x35:
        case 0x2D:
        case 0x3D:
        case 0x39:
        case 0x21:
        case 0x31:
            and(cpu, inst.mode);
            cpu->program_counter = original_pc_state + inst.bytes;
            continue;
        
        case 0x0A:
            asl_acc(cpu);
            cpu->program_counter = original_pc_state + inst.bytes;
            continue;

        case 0x06:
        case 0x16:
        case 0x0E:
        case 0x1E:
            asl(cpu, inst.mode);
            cpu->program_counter = original_pc_state + inst.bytes;
            continue;

        case 0x90:
            uint8_t displacement = bcc(cpu);
            displace(cpu, displacement, inst);
            continue;
        
        case 0x00:
            brk();
            return;

        case 0xE8:
            inx(cpu);
            cpu->program_counter = original_pc_state + inst.bytes;
            continue;

        case 0xA9:
        case 0xA5:
        case 0xB5:
        case 0xAD:
        case 0xBD:
        case 0xB9:
        case 0xA1:
        case 0xB1:
            lda(cpu, inst.mode);
            cpu->program_counter = original_pc_state + inst.bytes;
            continue;

        case 0xAA:
            tax(cpu);
            cpu->program_counter = original_pc_state + inst.bytes;
            continue;
        
        default:
            continue;
        }
    }
}

void displace(CPU *cpu, uint8_t displacement, Instruction inst)
{
    if (displacement != 0)
    {
        cpu->program_counter += displacement;
    }
    else
    {
        cpu->program_counter += inst.bytes - 1;
    }
}