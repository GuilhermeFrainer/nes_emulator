#include "../lib/cpu.h"
#include "../lib/instructions.h"
#include "../lib/io.h"
#include "../lib/bus.h"
#include "../lib/cartridge.h"

#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <stdio.h>

CPU *new_cpu(ROM *rom)
{
    CPU *cpu = malloc(sizeof(CPU));
    cpu->status = 0;
    cpu->program_counter = 0;
    cpu->stack_pointer = STACK_RESET;
    cpu->reg_a = 0;
    cpu->reg_x = 0;
    cpu->reg_y = 0;
    cpu->bus = new_bus(rom);
    return cpu;
}

void destroy_cpu(CPU *cpu)
{
    free(cpu->bus->rom->prg_rom);
    free(cpu->bus->rom->chr_rom);
    free(cpu->bus->rom);
    free(cpu->bus->ppu);
    free(cpu->bus);
    free(cpu);
}

uint8_t mem_read(CPU *cpu, uint16_t addr)
{
    return bus_mem_read(cpu->bus, addr);
}

// Deals with NES's little-endianess
uint16_t mem_read_u16(CPU *cpu, uint16_t addr)
{
    uint16_t low = mem_read(cpu, addr);
    uint16_t high = mem_read(cpu, addr + 1);
    high = high << 8;
    return high | low;
}

void mem_write(CPU *cpu, uint8_t value, uint16_t addr)
{
    bus_mem_write(cpu->bus, value, addr);
}

void mem_write_u16(CPU *cpu, uint16_t value, uint16_t addr)
{
    uint8_t low = value & 0xFF;
    uint8_t high = value >> 8;
    mem_write(cpu, low, addr);
    mem_write(cpu, high, addr + 1);
}

void reset(CPU *cpu)
{
    cpu->reg_a = 0;
    cpu->reg_x = 0;
    cpu->reg_y = 0;
    cpu->status = 0b00100100;
    cpu->program_counter = mem_read_u16(cpu, PROGRAM_START_ADDR);
    cpu->stack_pointer = STACK_RESET;
}

void load(CPU *cpu)
{
    uint8_t low = PROGRAM_START & 0xFF;
    uint8_t high = PROGRAM_START >> 8;
    
    cpu->bus->rom->prg_rom[PROGRAM_START_ADDR] = low;
    cpu->bus->rom->prg_rom[PROGRAM_START_ADDR] = high;

    cpu->program_counter = PROGRAM_START;
}

void run(CPU *cpu, SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_Event event;
    
    while (1)
    {
        Interrupt interrupt_type = bus_poll_for_interrupt(cpu->bus);
        switch (interrupt_type)
        {
            case NMI:
                interrupt(cpu, interrupt_type);
                break;
            case IRQ:
                if (!is_set(cpu, INTERRUPT_FLAG))
                {
                    interrupt(cpu, interrupt_type);
                }
                break;
            case None:
                break;
        }

        uint8_t opcode = mem_read(cpu, cpu->program_counter);
        mem_write(cpu, (rand() % 256) + 1, RAND_NUM_ADDR);
        interpret(cpu, opcode);
        if (opcode == 0x00)
        {
            return;
        }
        if (handle_input(cpu, &event))
        {
            return;
        }
        if (read_screen_state(cpu, buffer))
        {
            SDL_RenderClear(renderer);
            SDL_UpdateTexture(texture, NULL, buffer, GAME_WIDTH * 3);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(50);
        }
    }
}   
        
void interpret(CPU *cpu, uint8_t opcode)
{
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
            break;

        case 0x29:
        case 0x25:
        case 0x35:
        case 0x2D:
        case 0x3D:
        case 0x39:
        case 0x21:
        case 0x31:
            and(cpu, inst.mode);
            break;
        
        case 0x0A:
            asl_acc(cpu);
            break;

        case 0x06:
        case 0x16:
        case 0x0E:
        case 0x1E:
            asl(cpu, inst.mode);
            break;

        case 0x90:
            bcc(cpu);
            branch = true;
            break;

        case 0xB0:
            bcs(cpu);
            branch = true;
            break;

        case 0xF0:
            beq(cpu);
            branch = true;
            break;

        case 0x24:
        case 0x2C:
            bit(cpu, inst.mode);
            break;
        
        case 0x30:
            bmi(cpu);
            branch = true;
            break;

        case 0xD0:
            bne(cpu);
            branch = true;
            break;

        case 0x10:
            bpl(cpu);
            branch = true;
            break;

        case 0x00:
            brk(cpu);
            return;

        case 0x50:
            bvc(cpu);
            branch = true;
            break;

        case 0x70:
            bvs(cpu);
            branch = true;
            break;

        case 0x18:
            clc(cpu);
            break;

        case 0xD8:
            cld(cpu);
            break;

        case 0x58:
            cli(cpu);
            break;

        case 0xB8:
            clv(cpu);
            break;

        case 0xC9:
        case 0xC5:
        case 0xD5:
        case 0xCD:
        case 0xDD:
        case 0xD9:
        case 0xC1:
        case 0xD1:
            cmp(cpu, inst.mode);
            break;

        case 0xE0:
        case 0xE4:
        case 0xEC:
            cpx(cpu, inst.mode);
            break;

        case 0xC0:
        case 0xC4:
        case 0xCC:
            cpy(cpu, inst.mode);
            break;

        case 0xC6:
        case 0xD6:
        case 0xCE:
        case 0xDE:
            dec(cpu, inst.mode);
            break;

        case 0xCA:
            dex(cpu);
            break;

        case 0x88:
            dey(cpu);
            break;

        case 0x49:
        case 0x45:
        case 0x55:
        case 0x4D:
        case 0x5D:
        case 0x59:
        case 0x41:
        case 0x51:
            eor(cpu, inst.mode);
            break;

        case 0xE6:
        case 0xF6:
        case 0xEE:
        case 0xFE:
            inc(cpu, inst.mode);
            break;

        case 0xE8:
            inx(cpu);
            break;

        case 0xC8:
            iny(cpu);
            break;

        case 0x4C:
        case 0x6C:
            jmp(cpu, inst.mode);
            branch = true;
            break;

        case 0x20:
            jsr(cpu);
            branch = true;
            break;

        case 0xA9:
        case 0xA5:
        case 0xB5:
        case 0xAD:
        case 0xBD:
        case 0xB9:
        case 0xA1:
        case 0xB1:
            lda(cpu, inst.mode);
            break;

        case 0xA2:
        case 0xA6:
        case 0xB6:
        case 0xAE:
        case 0xBE:
            ldx(cpu, inst.mode);
            break;

        case 0xA0:
        case 0xA4:
        case 0xB4:
        case 0xAC:
        case 0xBC:
            ldy(cpu, inst.mode);
            break;

        case 0x4A:
            lsr_acc(cpu);
            break;
        
        case 0x46:
        case 0x56:
        case 0x4E:
        case 0x5E:
            lsr(cpu, inst.mode);
            break;

        case 0xEA:
            // NOP
            break;

        case 0x09:
        case 0x05:
        case 0x15:
        case 0x0D:
        case 0x1D:
        case 0x19:
        case 0x01:
        case 0x11:
            ora(cpu, inst.mode);
            break;

        case 0x48:
            pha(cpu);
            break;

        case 0x08:
            php(cpu);
            break;

        case 0x68:
            pla(cpu);
            break;

        case 0x28:
            plp(cpu);
            break;

        case 0x2A:
            rol_acc(cpu);
            break;

        case 0x26:
        case 0x36:
        case 0x2E:
        case 0x3E:
            rol(cpu, inst.mode);
            break;

        case 0x6A:
            ror_acc(cpu);
            break;

        case 0x66:
        case 0x76:
        case 0x6E:
        case 0x7E:
            ror(cpu, inst.mode);
            break;

        case 0x40:
            rti(cpu);
            break;
        
        case 0x60:
            rts(cpu);
            branch = true;
            break;

        case 0xE9:
        case 0xE5:
        case 0xF5:
        case 0xED:
        case 0xFD:
        case 0xF9:
        case 0xE1:
        case 0xF1:
            sbc(cpu, inst.mode);
            break;

        case 0x38:
            sec(cpu);
            break;

        case 0xF8:
            sed(cpu);
            break;

        case 0x78:
            sei(cpu);
            break;

        case 0x85:
        case 0x95:
        case 0x8D:
        case 0x9D:
        case 0x99:
        case 0x81:
        case 0x91:
            sta(cpu, inst.mode);
            break;

        case 0x86:
        case 0x96:
        case 0x8E:
            stx(cpu, inst.mode);
            break;

        case 0x84:
        case 0x94:
        case 0x8C:
            sty(cpu, inst.mode);
            break;

        case 0xAA:
            tax(cpu);
            break;

        case 0xA8:
            tay(cpu);
            break;

        case 0xBA:
            tsx(cpu);
            break;

        case 0x8A:
            txa(cpu);
            break;

        case 0x9A:
            txs(cpu);
            break;

        case 0x98:
            tya(cpu);
            break;

        // Unofficial opcodes

        case 0x0B:
        case 0x2B:
            anc(cpu, inst.mode);
            break;

        case 0x6B:
            arr(cpu, inst.mode);
            break;

        case 0x4B:
            asr(cpu, inst.mode);
            break;

        case 0xC7:
		case 0xD7:
		case 0xCF:
		case 0xDF:
		case 0xDB:
		case 0xC3:
		case 0xD3:
            dcp(cpu, inst.mode);
            break;

        case 0x82:
		case 0x89:
		case 0xC2:
		case 0xE2:
		case 0x80:
        case 0x04:
        case 0x44:
        case 0x64:
        case 0x14:
		case 0x34:
		case 0x54:
		case 0x74:
		case 0xD4:
		case 0xF4:
            // DOP (Double NOP)
            break;

        case 0xE7:
		case 0xF7:
		case 0xEF:
		case 0xFF:
		case 0xFB:
		case 0xE3:
		case 0xF3:
            isb(cpu, inst.mode);
            break;

        case 0x02:
		case 0x12:
		case 0x22:
		case 0x32:
		case 0x42:
		case 0x52:
		case 0x62:
		case 0x72:
		case 0x92:
		case 0xB2:
		case 0xD2:
		case 0xF2:
            // KIL
            return;

        case 0xBB:
            lae(cpu, inst.mode);
            break;

        case 0xA7:
        case 0xB7:
        case 0xAF:
        case 0xBF:
        case 0xA3:
        case 0xB3:
            lax(cpu, inst.mode);
            break;

        case 0x1A:
		case 0x3A:
		case 0x5A:
		case 0x7A:
		case 0xDA:
		case 0xFA:
            // NOP
            break;

        case 0x27:
		case 0x37:
		case 0x2F:
		case 0x3F:
		case 0x3B:
		case 0x23:
		case 0x33:
            rla(cpu, inst.mode);
            break;

        case 0x67:
		case 0x77:
		case 0x6F:
		case 0x7F:
		case 0x7B:
		case 0x63:
		case 0x73:
            rra(cpu, inst.mode);
            break;

        case 0x87:
        case 0x97:
        case 0x8F:
        case 0x83:
            sax(cpu, inst.mode);
            break;

        case 0xCB:
            sbx(cpu, inst.mode);
            break;

        case 0xEB:
            sbc(cpu, inst.mode);
            break;

        case 0x9F:
        case 0x93:
            sha(cpu, inst.mode);
            break;

        case 0x9B:
            shs(cpu, inst.mode);
            break;

        case 0x9E:
            shx(cpu, inst.mode);
            break;

        case 0x9C:
            shy(cpu, inst.mode);
            break;

        case 0x07:
		case 0x17:
		case 0x0F:
		case 0x1F:
		case 0x1B:
		case 0x03:
		case 0x13:
            slo(cpu, inst.mode);
            break;

        case 0x47:
		case 0x57:
		case 0x4F:
		case 0x5F:
		case 0x5B:
		case 0x43:
		case 0x53:
            sre(cpu, inst.mode);
            break;

        case 0x1C:
		case 0x3C:
		case 0x5C:
		case 0x7C:
		case 0xDC:
		case 0xFC:
            // TOP (Triple NOP)
            break;

        case 0x8B:
            xaa(cpu, inst.mode);
            break;
        
        default:
            fprintf(stderr, "Attempted to run unknown opcode: %x\n", opcode);
            break;
    }
    if (!branch)
    {
        cpu->program_counter = original_pc_state + inst.bytes;
    }
}

// Execute interrupt
void interrupt(CPU *cpu, Interrupt interrupt_type)
{
    stack_push_u16(cpu, cpu->program_counter);
    stack_push(cpu, cpu->status);
    set_flag(cpu, INTERRUPT_FLAG);

    bus_tick(cpu->bus, 7); // Interrupt takes 7 cycles
    // Places interrupt vector's address on the program counter
    switch (interrupt_type)
    {
        case IRQ:
            cpu->program_counter = mem_read_u16(cpu, 0xFFFE);
            break;
        case NMI:
            cpu->program_counter = mem_read_u16(cpu, 0xFFFA);
            break;
        case None:
            break;
    }
    bus_clear_interrupt(cpu->bus);
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

// Flag functions

void set_flag(CPU *cpu, uint8_t flag)
{
    cpu->status |= flag;
}

void unset_flag(CPU *cpu, uint8_t flag)
{
    cpu->status &= ~flag;
}

bool is_set(CPU *cpu, uint8_t flag)
{
    return ((cpu->status & flag) != 0) ? true : false;
}

// Stack functions

uint16_t get_stack_addr(CPU *cpu)
{
    uint16_t stack_pointer = (uint16_t) cpu->stack_pointer;
    return STACK | stack_pointer;
}

void stack_push(CPU *cpu, uint8_t value)
{
    uint16_t addr = get_stack_addr(cpu);
    mem_write(cpu, value, addr);
    cpu->stack_pointer--;
}

void stack_push_u16(CPU *cpu, uint16_t value)
{
    uint8_t high = value >> 8;
    uint8_t low = value & 0xFF;
    stack_push(cpu, high);
    stack_push(cpu, low);
}

uint8_t stack_pull(CPU *cpu)
{
    cpu->stack_pointer++;
    uint16_t addr = get_stack_addr(cpu);
    uint8_t value = mem_read(cpu, addr);
    return value;
}

uint16_t stack_pull_u16(CPU *cpu)
{
    uint16_t low = (uint16_t) stack_pull(cpu);
    uint16_t high = (uint16_t) stack_pull(cpu) << 8;
    return high | low;
}
