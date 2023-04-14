#include "cpu.h"
#include "instructions.h"

CPU new_cpu(void)
{
    CPU cpu;
    cpu.status = 0;
    cpu.program_counter = 0;
    cpu.stack_pointer = 0;
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
        bool branch = false;

        switch (opcode)
        {
            case 0x69:
            case 0x65:
            case 0x75:
            case 0x6D:
            case 0x7D:
            case 0x79:
            case 0x61:
            case 0x71:
                adc(cpu, inst.mode);
                continue;

            case 0x29:
            case 0x25:
            case 0x35:
            case 0x2D:
            case 0x3D:
            case 0x39:
            case 0x21:
            case 0x31:
                and(cpu, inst.mode);
                continue;
            
            case 0x0A:
                asl_acc(cpu);
                continue;

            case 0x06:
            case 0x16:
            case 0x0E:
            case 0x1E:
                asl(cpu, inst.mode);
                continue;

            case 0x90:
                bcc(cpu);
                branch = true;
                continue;

            case 0xB0:
                bcs(cpu);
                branch = true;
                continue;

            case 0xF0:
                beq(cpu);
                branch = true;
                continue;

            case 0x24:
            case 0x2C:
                bit(cpu, inst.mode);
                continue;
            
            case 0x30:
                bmi(cpu);
                branch = true;
                continue;

            case 0xD0:
                bne(cpu);
                branch = true;
                continue;

            case 0x10:
                bpl(cpu);
                branch = true;
                continue;

            case 0x00:
                brk(cpu);
                return;

            case 0x50:
                bvc(cpu);
                branch = true;
                continue;

            case 0x70:
                bvs(cpu);
                branch = true;
                continue;

            case 0x18:
                clc(cpu);
                continue;

            case 0xD8:
                cld(cpu);
                continue;

            case 0x58:
                cli(cpu);
                continue;

            case 0xB8:
                clv(cpu);
                continue;

            case 0xC9:
            case 0xC5:
            case 0xD5:
            case 0xCD:
            case 0xDD:
            case 0xD9:
            case 0xC1:
            case 0xD1:
                cmp(cpu, inst.mode);
                continue;

            case 0xE0:
            case 0xE4:
            case 0xEC:
                cpx(cpu, inst.mode);
                continue;

            case 0xC0:
            case 0xC4:
            case 0xCC:
                cpy(cpu, inst.mode);
                continue;

            case 0xC6:
            case 0xD6:
            case 0xCE:
            case 0xDE:
                dec(cpu, inst.mode);
                continue;

            case 0xCA:
                dex(cpu);
                continue;

            case 0x88:
                dey(cpu);
                continue;

            case 0x49:
            case 0x45:
            case 0x55:
            case 0x4D:
            case 0x5D:
            case 0x59:
            case 0x41:
            case 0x51:
                eor(cpu, inst.mode);
                continue;

            case 0xE6:
            case 0xF6:
            case 0xEE:
            case 0xFE:
                inc(cpu, inst.mode);
                continue;

            case 0xE8:
                inx(cpu);
                continue;

            case 0xC8:
                iny(cpu);
                continue;

            case 0x4C:
            case 0x6C:
                jmp(cpu, inst.mode);
                branch = true;
                continue;

            case 0x20:
                jsr(cpu);
                branch = true;
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
                continue;

            case 0xA2:
            case 0xA6:
            case 0xB6:
            case 0xAE:
            case 0xBE:
                ldx(cpu, inst.mode);
                continue;

            case 0xA0:
            case 0xA4:
            case 0xB4:
            case 0xAC:
            case 0xBC:
                ldy(cpu, inst.mode);
                continue;

            case 0x4A:
                lsr_acc(cpu);
                continue;
            
            case 0x46:
            case 0x56:
            case 0x4E:
            case 0x5E:
                lsr(cpu, inst.mode);
                continue;

            case 0xEA:
                // NOP
                continue;

            case 0x09:
            case 0x05:
            case 0x15:
            case 0x0D:
            case 0x1D:
            case 0x19:
            case 0x01:
            case 0x11:
                ora(cpu, inst.mode);
                continue;

            case 0x48:
                pha(cpu);
                continue;

            case 0x08:
                php(cpu);
                continue;

            case 0x68:
                pla(cpu);
                continue;

            case 0x28:
                plp(cpu);
                continue;

            case 0x2A:
                rol_acc(cpu);
                continue;

            case 0x26:
            case 0x36:
            case 0x2E:
            case 0x3E:
                rol(cpu, inst.mode);
                continue;

            case 0x6A:
                ror_acc(cpu);
                continue;

            case 0x66:
            case 0x76:
            case 0x6E:
            case 0x7E:
                ror(cpu, inst.mode);
                continue;

            case 0x40:
                rti(cpu);
                continue;
            
            case 0x60:
                rts(cpu);
                continue;

            case 0xE9:
            case 0xE5:
            case 0xF5:
            case 0xED:
            case 0xFD:
            case 0xF9:
            case 0xE1:
            case 0xF1:
                sbc(cpu, inst.mode);
                continue;

            case 0x38:
                sec(cpu);
                continue;

            case 0xF8:
                sed(cpu);
                continue;

            case 0x78:
                sei(cpu);
                continue;

            case 0x85:
            case 0x95:
            case 0x8D:
            case 0x9D:
            case 0x99:
            case 0x81:
            case 0x91:
                sta(cpu, inst.mode);
                continue;

            case 0x86:
            case 0x96:
            case 0x8E:
                stx(cpu, inst.mode);
                continue;

            case 0x84:
            case 0x94:
            case 0x8C:
                sty(cpu, inst.mode);
                continue;

            case 0xAA:
                tax(cpu);
                continue;

            case 0xA8:
                tay(cpu);
                continue;

            case 0xBA:
                tsx(cpu);
                continue;

            case 0x8A:
                txa(cpu);
                continue;

            case 0x9A:
                txs(cpu);
                continue;

            case 0x98:
                tya(cpu);
                continue;
            
            default:
                continue;
        }
        if (!branch)
        {
            cpu->program_counter = original_pc_state + inst.bytes;
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