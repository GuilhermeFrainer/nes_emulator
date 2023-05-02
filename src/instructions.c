#include "../lib/instructions.h"
#include "../lib/cpu.h"

Instruction inst_list[0xFF];

void populate_inst_list(void)
{
    for (int i = 0x00; i < 0xFF; i++)
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
        // In alphabetical order of mnemonics
        // Some are grouped by functionality, but that is just an alphabetical coincidence
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

        // Branch instructions part 1
        case 0x90: inst = (Instruction) {0x90, "BCC", 2, 2, Implied}; return inst;
        case 0xB0: inst = (Instruction) {0xB0, "BCS", 2, 2, Implied}; return inst;
        case 0xF0: inst = (Instruction) {0xF0, "BEQ", 2, 2, Implied}; return inst;

        case 0x24: inst = (Instruction) {0x24, "BIT", 2, 3, ZeroPage}; return inst;
        case 0x2C: inst = (Instruction) {0x2C, "BIT", 3, 4, Absolute}; return inst;

        // Branch instructions part 2
        case 0x30: inst = (Instruction) {0x30, "BMI", 2, 2, Implied}; return inst;
        case 0xD0: inst = (Instruction) {0xD0, "BNE", 2, 2, Implied}; return inst;
        case 0x10: inst = (Instruction) {0x10, "BPL", 2, 2, Implied}; return inst;

        case 0x00: inst = (Instruction) {0x00, "BRK", 1, 7, Implied}; return inst;

        // Branch instructions part 3
        case 0x50: inst = (Instruction) {0x50, "BVC", 2, 2, Implied}; return inst;
        case 0x70: inst = (Instruction) {0x70, "BVS", 2, 2, Implied}; return inst;

        // Clear instructions
        case 0x18: inst = (Instruction) {0x18, "CLC", 1, 2, Implied}; return inst;
        case 0xD8: inst = (Instruction) {0xD8, "CLD", 1, 2, Implied}; return inst;
        case 0x58: inst = (Instruction) {0x58, "CLI", 1, 2, Implied}; return inst;
        case 0xB8: inst = (Instruction) {0xB8, "CLV", 1, 2, Implied}; return inst;

        // Compare instructions
        case 0xC9: inst = (Instruction) {0xC9, "CMP", 2, 2, Immediate}; return inst;
        case 0xC5: inst = (Instruction) {0xC5, "CMP", 2, 3, ZeroPage}; return inst;
        case 0xD5: inst = (Instruction) {0xD5, "CMP", 2, 4, ZeroPageX}; return inst;
        case 0xCD: inst = (Instruction) {0xCD, "CMP", 3, 4, Absolute}; return inst;
        case 0xDD: inst = (Instruction) {0xDD, "CMP", 3, 4, AbsoluteX}; return inst;
        case 0xD9: inst = (Instruction) {0xD9, "CMP", 3, 4, AbsoluteY}; return inst;
        case 0xC1: inst = (Instruction) {0xC1, "CMP", 2, 6, IndirectX}; return inst;
        case 0xD1: inst = (Instruction) {0xD1, "CMP", 2, 5, IndirectY}; return inst;

        case 0xE0: inst = (Instruction) {0xE0, "CPX", 2, 2, Immediate}; return inst;
        case 0xE4: inst = (Instruction) {0xE4, "CPX", 2, 3, ZeroPage}; return inst;
        case 0xEC: inst = (Instruction) {0xEC, "CPX", 3, 4, Absolute}; return inst;

        case 0xC0: inst = (Instruction) {0xC0, "CPY", 2, 2, Immediate}; return inst;
        case 0xC4: inst = (Instruction) {0xC4, "CPY", 2, 3, ZeroPage}; return inst;
        case 0xCC: inst = (Instruction) {0xCC, "CPY", 3, 4, Absolute}; return inst;

        // Decrement instructions
        case 0xC6: inst = (Instruction) {0xC6, "DEC", 2, 5, ZeroPage}; return inst;
        case 0xD6: inst = (Instruction) {0xD6, "DEC", 2, 6, ZeroPageX}; return inst;
        case 0xCE: inst = (Instruction) {0xCE, "DEC", 3, 6, Absolute}; return inst;
        case 0xDE: inst = (Instruction) {0xDE, "DEC", 3, 7, AbsoluteX}; return inst;

        case 0xCA: inst = (Instruction) {0xCA, "DEX", 1, 2, Implied}; return inst;
        case 0x88: inst = (Instruction) {0x88, "DEY", 1, 2, Implied}; return inst;

        case 0x49: inst = (Instruction) {0x49, "EOR", 2, 2, Immediate}; return inst;
        case 0x45: inst = (Instruction) {0x45, "EOR", 2, 3, ZeroPage}; return inst;
        case 0x55: inst = (Instruction) {0x55, "EOR", 2, 4, ZeroPageX}; return inst;
        case 0x4D: inst = (Instruction) {0x4D, "EOR", 3, 4, Absolute}; return inst;
        case 0x5D: inst = (Instruction) {0x5D, "EOR", 3, 4, AbsoluteX}; return inst;
        case 0x59: inst = (Instruction) {0x59, "EOR", 3, 4, AbsoluteY}; return inst;
        case 0x41: inst = (Instruction) {0x41, "EOR", 2, 6, IndirectX}; return inst;
        case 0x51: inst = (Instruction) {0x51, "EOR", 2, 5, IndirectY}; return inst;

        // Increment instructions
        case 0xE6: inst = (Instruction) {0xE6, "INC", 2, 5, ZeroPage}; return inst;
        case 0xF6: inst = (Instruction) {0xF6, "INC", 2, 6, ZeroPageX}; return inst;
        case 0xEE: inst = (Instruction) {0xEE, "INC", 3, 6, Absolute}; return inst;
        case 0xFE: inst = (Instruction) {0xFE, "INC", 3, 7, AbsoluteX}; return inst;

        case 0xE8: inst = (Instruction) {0xE8, "INX", 1, 2, Implied}; return inst;

        case 0xC8: inst = (Instruction) {0xC8, "INY", 1, 2, Implied}; return inst;

        // Jump instructions
        case 0x4C: inst = (Instruction) {0x4C, "JMP", 3, 3, Absolute}; return inst;
        case 0x6C: inst = (Instruction) {0x6C, "JMP", 3, 5, Indirect}; return inst;

        case 0x20: inst = (Instruction) {0x20, "JSR", 3, 6, Absolute}; return inst;

        // Load instructions
        case 0xA9: inst = (Instruction) {0xA9, "LDA", 2, 2, Immediate}; return inst;
        case 0xA5: inst = (Instruction) {0xA5, "LDA", 2, 3, ZeroPage}; return inst;
        case 0xB5: inst = (Instruction) {0xB5, "LDA", 2, 4, ZeroPageX}; return inst;
        case 0xAD: inst = (Instruction) {0xAD, "LDA", 3, 4, Absolute}; return inst;
        case 0xBD: inst = (Instruction) {0xBD, "LDA", 3, 4, AbsoluteX}; return inst;
        case 0xB9: inst = (Instruction) {0xB9, "LDA", 3, 4, AbsoluteY}; return inst;
        case 0xA1: inst = (Instruction) {0xA1, "LDA", 2, 6, IndirectX}; return inst;
        case 0xB1: inst = (Instruction) {0xB1, "LDA", 2, 5, IndirectY}; return inst;

        case 0xA2: inst = (Instruction) {0xA2, "LDX", 2, 2, Immediate}; return inst;
        case 0xA6: inst = (Instruction) {0xA6, "LDX", 2, 3, ZeroPage}; return inst;
        case 0xB6: inst = (Instruction) {0xB6, "LDX", 2, 4, ZeroPageY}; return inst;
        case 0xAE: inst = (Instruction) {0xAE, "LDX", 3, 4, Absolute}; return inst;
        case 0xBE: inst = (Instruction) {0xBE, "LDX", 3, 4, AbsoluteY}; return inst;

        case 0xA0: inst = (Instruction) {0xA0, "LDY", 2, 2, Immediate}; return inst;
        case 0xA4: inst = (Instruction) {0xA4, "LDY", 2, 3, ZeroPage}; return inst;
        case 0xB4: inst = (Instruction) {0xB4, "LDY", 2, 4, ZeroPageX}; return inst;
        case 0xAC: inst = (Instruction) {0xAC, "LDY", 3, 4, Absolute}; return inst;
        case 0xBC: inst = (Instruction) {0xBC, "LDY", 3, 4, AbsoluteX}; return inst;

        case 0x4A: inst = (Instruction) {0x4A, "LSR", 1, 2, Implied}; return inst;
        case 0x46: inst = (Instruction) {0x46, "LSR", 2, 5, ZeroPage}; return inst;
        case 0x56: inst = (Instruction) {0x56, "LSR", 2, 6, ZeroPageX}; return inst;
        case 0x4E: inst = (Instruction) {0x4E, "LSR", 3, 6, Absolute}; return inst;
        case 0x5E: inst = (Instruction) {0x5E, "LSR", 3, 7, AbsoluteX}; return inst;

        case 0xEA: inst = (Instruction) {0xEA, "NOP", 1, 2, Implied}; return inst;

        case 0x09: inst = (Instruction) {0x09, "ORA", 2, 2, Immediate}; return inst;
        case 0x05: inst = (Instruction) {0x05, "ORA", 2, 3, ZeroPage}; return inst;
        case 0x15: inst = (Instruction) {0x15, "ORA", 2, 4, ZeroPageX}; return inst;
        case 0x0D: inst = (Instruction) {0x0D, "ORA", 3, 4, Absolute}; return inst;
        case 0x1D: inst = (Instruction) {0x1D, "ORA", 3, 4, AbsoluteX}; return inst;
        case 0x19: inst = (Instruction) {0x19, "ORA", 3, 4, AbsoluteY}; return inst;
        case 0x01: inst = (Instruction) {0x01, "ORA", 2, 6, IndirectX}; return inst;
        case 0x11: inst = (Instruction) {0x11, "ORA", 2, 5, IndirectY}; return inst;

        // Push instructions
        case 0x48: inst = (Instruction) {0x48, "PHA", 1, 3, Implied}; return inst;
        case 0x08: inst = (Instruction) {0x08, "PHP", 1, 3, Implied}; return inst;

        // Pull instructions
        case 0x68: inst = (Instruction) {0x68, "PLA", 1, 4, Implied}; return inst;
        case 0x28: inst = (Instruction) {0x28, "PLP", 1, 4, Implied}; return inst;

        // Rotate instructions
        case 0x2A: inst = (Instruction) {0x2A, "ROL", 1, 2, Implied}; return inst;
        case 0x26: inst = (Instruction) {0x26, "ROL", 2, 5, ZeroPage}; return inst;
        case 0x36: inst = (Instruction) {0x36, "ROL", 2, 6, ZeroPageX}; return inst;
        case 0x2E: inst = (Instruction) {0x2E, "ROL", 3, 6, Absolute}; return inst;
        case 0x3E: inst = (Instruction) {0x3E, "ROL", 3, 7, AbsoluteX}; return inst;

        case 0x6A: inst = (Instruction) {0x6A, "ROR", 1, 2, Implied}; return inst;
        case 0x66: inst = (Instruction) {0x66, "ROR", 2, 5, ZeroPage}; return inst;
        case 0x76: inst = (Instruction) {0x76, "ROR", 2, 6, ZeroPageX}; return inst;
        case 0x6E: inst = (Instruction) {0x6E, "ROR", 3, 6, Absolute}; return inst;
        case 0x7E: inst = (Instruction) {0x7E, "ROR", 3, 7, AbsoluteX}; return inst;

        // Return instructions
        case 0x40: inst = (Instruction) {0x40, "RTI", 1, 6, Implied}; return inst;
        case 0x60: inst = (Instruction) {0x60, "RTS", 1, 6, Implied}; return inst;

        case 0xE9: inst = (Instruction) {0xE9, "SBC", 2, 2, Immediate}; return inst;
        case 0xE5: inst = (Instruction) {0xE5, "SBC", 2, 3, ZeroPage}; return inst;
        case 0xF5: inst = (Instruction) {0xF5, "SBC", 2, 4, ZeroPageX}; return inst;
        case 0xED: inst = (Instruction) {0xED, "SBC", 3, 4, Absolute}; return inst;
        case 0xFD: inst = (Instruction) {0xFD, "SBC", 3, 4, AbsoluteX}; return inst;
        case 0xF9: inst = (Instruction) {0xF9, "SBC", 3, 4, AbsoluteY}; return inst;
        case 0xE1: inst = (Instruction) {0xE1, "SBC", 2, 6, IndirectX}; return inst;
        case 0xF1: inst = (Instruction) {0xF1, "SBC", 2, 5, IndirectY}; return inst;

        // Set flag intructions
        case 0x38: inst = (Instruction) {0x38, "SEC", 1, 2, Implied}; return inst;
        case 0xF8: inst = (Instruction) {0xF8, "SED", 1, 2, Implied}; return inst;
        case 0x78: inst = (Instruction) {0x78, "SEI", 1, 2, Implied}; return inst;

        // Store instructions
        case 0x85: inst = (Instruction) {0x85, "STA", 2, 3, ZeroPage}; return inst;
        case 0x95: inst = (Instruction) {0x95, "STA", 2, 4, ZeroPageX}; return inst;
        case 0x8D: inst = (Instruction) {0x8D, "STA", 3, 4, Absolute}; return inst;
        case 0x9D: inst = (Instruction) {0x9D, "STA", 3, 5, AbsoluteX}; return inst;
        case 0x99: inst = (Instruction) {0x99, "STA", 3, 5, AbsoluteY}; return inst;
        case 0x81: inst = (Instruction) {0x81, "STA", 2, 6, IndirectX}; return inst;
        case 0x91: inst = (Instruction) {0x91, "STA", 2, 6, IndirectY}; return inst;

        case 0x86: inst = (Instruction) {0x86, "STX", 2, 3, ZeroPage}; return inst;
        case 0x96: inst = (Instruction) {0x96, "STX", 2, 4, ZeroPageY}; return inst;
        case 0x8E: inst = (Instruction) {0x8E, "STX", 3, 4, Absolute}; return inst;

        case 0x84: inst = (Instruction) {0x84, "STY", 2, 3, ZeroPage}; return inst;
        case 0x94: inst = (Instruction) {0x94, "STY", 2, 4, ZeroPageX}; return inst;
        case 0x8C: inst = (Instruction) {0x8C, "STY", 3, 4, Absolute}; return inst;
        
        // Transfer instructions
        case 0xAA: inst = (Instruction) {0xAA, "TAX", 1, 2, Implied}; return inst;
        case 0xA8: inst = (Instruction) {0xA8, "TAY", 1, 2, Implied}; return inst;
        case 0xBA: inst = (Instruction) {0xBA, "TSX", 1, 2, Implied}; return inst;
        case 0x8A: inst = (Instruction) {0x8A, "TXA", 1, 2, Implied}; return inst;
        case 0x9A: inst = (Instruction) {0x9A, "TXS", 1, 2, Implied}; return inst;
        case 0x98: inst = (Instruction) {0x98, "TYA", 1, 2, Implied}; return inst;

        default: inst = (Instruction) {0x00, "BRK", 1, 7, Implied}; return inst;
    }
}

// Instructions

void adc(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    add_with_carry(cpu, operand);
}

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

// Branch instructions part 1

void bcc(CPU *cpu)
{
    cpu->program_counter += (!is_set(cpu, CARRY_FLAG)) ? (int8_t) read_mem(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
}

void bcs(CPU *cpu)
{
    cpu->program_counter += (is_set(cpu, CARRY_FLAG)) ? (int8_t) read_mem(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
}

void beq(CPU *cpu)
{
    cpu->program_counter += (is_set(cpu, ZERO_FLAG)) ? (int8_t) read_mem(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
}

void bit(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);

    ((cpu->reg_a & operand) == 0) ? set_flag(cpu, ZERO_FLAG) : unset_flag(cpu, ZERO_FLAG);
    
    // Set V flag to operand 6th bit
    if ((operand & OVERFLOW_FLAG) != 0)
    {
        set_flag(cpu, OVERFLOW_FLAG);
    }
    else
    {
        unset_flag(cpu, OVERFLOW_FLAG);
    }

    // Set N flag to operand 7th bit
    if ((operand & NEGATIVE_FLAG) != 0)
    {
        set_flag(cpu, NEGATIVE_FLAG);
    }
    else
    {
        unset_flag(cpu, NEGATIVE_FLAG);
    }
}

// Branch instructions part 2

void bmi(CPU *cpu)
{
    cpu->program_counter += (is_set(cpu, NEGATIVE_FLAG)) ? (int8_t) read_mem(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
}

void bne(CPU *cpu)
{
    cpu->program_counter += (!is_set(cpu, ZERO_FLAG)) ? (int8_t) read_mem(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
}

void bpl(CPU *cpu)
{
    cpu->program_counter += (!is_set(cpu, NEGATIVE_FLAG)) ? (int8_t) read_mem(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
}

void brk(CPU *cpu)
{
    return;
}

// Branch instructions part 3

void bvc(CPU *cpu)
{
    cpu->program_counter += (!is_set(cpu, OVERFLOW_FLAG)) ? (int8_t) read_mem(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
}

void bvs(CPU *cpu)
{
    cpu->program_counter += (is_set(cpu, OVERFLOW_FLAG)) ? (int8_t) read_mem(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
}

// Clear flag instructions

void clc(CPU *cpu)
{
    unset_flag(cpu, CARRY_FLAG);
}

void cld(CPU *cpu)
{
    unset_flag(cpu, DECIMAL_FLAG);
}

void cli(CPU *cpu)
{
    unset_flag(cpu, INTERRUPT_FLAG);
}

void clv(CPU *cpu)
{
    unset_flag(cpu, OVERFLOW_FLAG);
}

// Compare instructions

void cmp(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    if (cpu->reg_a >= operand)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    uint8_t result = cpu->reg_a - operand;
    update_zero_and_negative_flags(cpu, result);
}

void cpx(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    if (cpu->reg_x >= operand)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    uint8_t result = cpu->reg_x - operand;
    update_zero_and_negative_flags(cpu, result);
}

void cpy(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    if (cpu->reg_y >= operand)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    uint8_t result = cpu->reg_y - operand;
    update_zero_and_negative_flags(cpu, result);
}

// Decrement instructions

void dec(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    operand--;
    write_mem(cpu, operand, addr);
    update_zero_and_negative_flags(cpu, operand);
}

void dex(CPU *cpu)
{
    cpu->reg_x--;
    update_zero_and_negative_flags(cpu, cpu->reg_x);
}

void dey(CPU *cpu)
{
    cpu->reg_y--;
    update_zero_and_negative_flags(cpu, cpu->reg_y);
}

void eor(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    cpu->reg_a ^= operand;
    update_zero_and_negative_flags(cpu, cpu->reg_a); 
}

// Increment instruction

void inc(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    operand++;
    write_mem(cpu, operand, addr);
    update_zero_and_negative_flags(cpu, operand);
}

void inx(CPU *cpu)
{
    cpu->reg_x++;
    update_zero_and_negative_flags(cpu, cpu->reg_x);
}

void iny(CPU *cpu)
{
    cpu->reg_y++;
    update_zero_and_negative_flags(cpu, cpu->reg_y);
}

// Jump instructions

void jmp(CPU *cpu, AddrMode mode)
{
    // Not implemented: page end bug
    cpu->program_counter = get_operand_addr(cpu, mode);
}

void jsr(CPU *cpu)
{
    stack_push_u16(cpu, cpu->program_counter + 2 - 1);
    uint16_t addr = read_mem_u16(cpu, cpu->program_counter);
    cpu->program_counter = addr;
}

// Load instructions

void lda(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t value = read_mem(cpu, addr);
    set_reg_a(cpu, value);
}

void ldx(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    set_reg_x(cpu, operand);
}

void ldy(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    set_reg_y(cpu, operand);
}

void lsr_acc(CPU *cpu)
{
    if ((cpu->reg_a & 1) != 0)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    set_reg_a(cpu, cpu->reg_a >> 1);
}

void lsr(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    if ((operand & 1) != 0)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    write_mem(cpu, operand >> 1, addr);
    update_zero_and_negative_flags(cpu, operand >> 1);
}

void ora(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    set_reg_a(cpu, cpu->reg_a | operand);
}

// Push instructions

void pha(CPU *cpu)
{
    stack_push(cpu, cpu->reg_a);
}

void php(CPU *cpu)
{
    stack_push(cpu, cpu->status);
}

// Pull instructions

void pla(CPU *cpu)
{
    uint8_t operand = stack_pull(cpu);
    set_reg_a(cpu, operand);
}

void plp(CPU *cpu)
{
    cpu->status = stack_pull(cpu);
}

// Rotate instructions

void rol_acc(CPU *cpu)
{
    uint8_t old_acc = cpu->reg_a;
    cpu->reg_a <<= 1;
    if (is_set(cpu, CARRY_FLAG))
    {
        cpu->reg_a |= 0x01;
    }
    
    if ((old_acc & 0x80) != 0)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
}

void rol(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    uint8_t new_value = operand << 1;
    if (is_set(cpu, CARRY_FLAG))
    {
        new_value |= 0x01;
    }
    if (operand & 0x80)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    write_mem(cpu, new_value, addr);
}

void ror_acc(CPU *cpu)
{
    uint8_t old_acc = cpu->reg_a;
    cpu->reg_a >>= 1;
    if (is_set(cpu, CARRY_FLAG))
    {
        cpu->reg_a |= 0x80;
    }
    if ((old_acc & 0x01) != 0)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
}

void ror(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    uint8_t new_value = operand >> 1;
    if (is_set(cpu, CARRY_FLAG))
    {
        new_value |= 0x80;
    }
    if ((operand & 0x01) != 0)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    write_mem(cpu, new_value, addr);
}

// Return instructions

void rti(CPU *cpu)
{
    uint8_t flags = stack_pull(cpu);
    uint16_t program_counter = stack_pull_u16(cpu);
    cpu->status = flags;
    cpu->program_counter = program_counter;
}

void rts(CPU *cpu)
{
    uint16_t program_counter = stack_pull_u16(cpu);
    cpu->program_counter = program_counter + 1;
}

void sbc(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = read_mem(cpu, addr);
    operand = ~operand + 1;
    add_with_carry(cpu, operand);
}

// Set flag instructions

void sec(CPU *cpu)
{
    set_flag(cpu, CARRY_FLAG);
}

void sed(CPU *cpu)
{
    set_flag(cpu, DECIMAL_FLAG);
}

void sei(CPU *cpu)
{
    set_flag(cpu, INTERRUPT_FLAG);
}

// Store instructions

void sta(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    write_mem(cpu, cpu->reg_a, addr);
}

void stx(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    write_mem(cpu, cpu->reg_x, addr);
}

void sty(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    write_mem(cpu, cpu->reg_y, addr);
}

// Transfer instructions

void tax(CPU *cpu)
{
    set_reg_x(cpu, cpu->reg_a);
}

void tay(CPU *cpu)
{
    set_reg_y(cpu, cpu->reg_a);
}

void tsx(CPU *cpu)
{
    set_reg_x(cpu, cpu->stack_pointer);
}

void txa(CPU *cpu)
{
    set_reg_a(cpu, cpu->reg_x);
}

void txs(CPU *cpu)
{
    cpu->stack_pointer = cpu->reg_x;
}

void tya(CPU *cpu)
{
    set_reg_a(cpu, cpu->reg_y);
}

// End instructions

// Utility functions

void update_zero_and_negative_flags(CPU *cpu, uint8_t result)
{
    // Update zero flag
    if (result == 0)
    {
        set_flag(cpu, ZERO_FLAG);
    }
    else
    {
        unset_flag(cpu, ZERO_FLAG);
    }
    // Update negative flag
    if ((result & 0b10000000) != 0)
    {
        set_flag(cpu, NEGATIVE_FLAG);
    }
    else
    {
        unset_flag(cpu, NEGATIVE_FLAG);
    }
}

void update_carry_flag(CPU *cpu, uint8_t value)
{
    if ((value >> 7) == 1)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
}

uint16_t get_operand_addr(CPU *cpu, AddrMode mode)
{
    switch (mode)
    {
        case Immediate: return cpu->program_counter;

        case ZeroPage: return read_mem(cpu, cpu->program_counter);
        case ZeroPageX: return read_mem(cpu, cpu->program_counter) + (int8_t) cpu->reg_x;
        case ZeroPageY: return read_mem(cpu, cpu->program_counter) + (int8_t) cpu->reg_y;

        case Absolute: return read_mem_u16(cpu, cpu->program_counter);
        case AbsoluteX: return read_mem_u16(cpu, cpu->program_counter) + (int8_t) cpu->reg_x;
        case AbsoluteY: return read_mem_u16(cpu, cpu->program_counter) + (int8_t) cpu->reg_y;

        case Indirect:
            uint16_t base_u16 = read_mem_u16(cpu, cpu->program_counter);
            return read_mem_u16(cpu, base_u16);
        case IndirectX:
            uint8_t base_u8 = read_mem(cpu, cpu->program_counter);
            return read_mem_u16(cpu, base_u8 + cpu->reg_x);
        case IndirectY:
            base_u16 = read_mem_u16(cpu, cpu->program_counter);
            return read_mem_u16(cpu, base_u16) + cpu->reg_y;
        default:
            return 0;
    }
}

// Made to implement both ADC and SBC more easily
void add_with_carry(CPU *cpu, uint8_t operand)
{
    uint8_t carry_in = is_set(cpu, CARRY_FLAG);
    uint16_t sum = (uint16_t) cpu->reg_a + operand + carry_in;

    // Check if carry flag must be set
    if (sum > 0xFF)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    
    // Check if overflow flag must be set
    // Check sources for a detailed explanation
    uint8_t result = (uint8_t) sum;
    if (((cpu->reg_a ^ result) & (operand ^ result) & 0x80) != 0)
    {
        set_flag(cpu, OVERFLOW_FLAG);
    }
    else
    {
        unset_flag(cpu, OVERFLOW_FLAG);
    }
    set_reg_a(cpu, result);
}