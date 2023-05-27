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
        case 0x69: inst = (Instruction) {0x69, "ADC\0", 2, 2, Immediate}; return inst;
        case 0x65: inst = (Instruction) {0x65, "ADC\0", 2, 3, ZeroPage}; return inst;
        case 0x75: inst = (Instruction) {0x75, "ADC\0", 2, 4, ZeroPageX}; return inst;
        case 0x6D: inst = (Instruction) {0x6D, "ADC\0", 3, 4, Absolute}; return inst;
        case 0x7D: inst = (Instruction) {0x7D, "ADC\0", 3, 4, AbsoluteX}; return inst;
        case 0x79: inst = (Instruction) {0x79, "ADC\0", 3, 4, AbsoluteY}; return inst;
        case 0x61: inst = (Instruction) {0x61, "ADC\0", 2, 6, IndirectX}; return inst;
        case 0x71: inst = (Instruction) {0x71, "ADC\0", 2, 5, IndirectY}; return inst;

        case 0x29: inst = (Instruction) {0x29, "AND\0", 2, 2, Immediate}; return inst;
        case 0x25: inst = (Instruction) {0x25, "AND\0", 2, 3, ZeroPage}; return inst;
        case 0x35: inst = (Instruction) {0x35, "AND\0", 2, 4, ZeroPageX}; return inst;
        case 0x2D: inst = (Instruction) {0x2D, "AND\0", 3, 4, Absolute}; return inst;
        case 0x3D: inst = (Instruction) {0x3D, "AND\0", 3, 4, AbsoluteX}; return inst;
        case 0x39: inst = (Instruction) {0x39, "AND\0", 3, 4, AbsoluteY}; return inst;
        case 0x21: inst = (Instruction) {0x21, "AND\0", 2, 6, IndirectX}; return inst;
        case 0x31: inst = (Instruction) {0x31, "AND\0", 2, 5, IndirectY}; return inst;

        case 0x0A: inst = (Instruction) {0x0A, "ASL\0", 1, 2, Implied}; return inst;
        case 0x06: inst = (Instruction) {0x06, "ASL\0", 2, 5, ZeroPage}; return inst;
        case 0x16: inst = (Instruction) {0x16, "ASL\0", 2, 6, ZeroPageX}; return inst;
        case 0x0E: inst = (Instruction) {0x0E, "ASL\0", 3, 6, Absolute}; return inst;
        case 0x1E: inst = (Instruction) {0x1E, "ASL\0", 3, 7, AbsoluteX}; return inst;

        // Branch instructions part 1
        case 0x90: inst = (Instruction) {0x90, "BCC\0", 2, 2, Relative}; return inst;
        case 0xB0: inst = (Instruction) {0xB0, "BCS\0", 2, 2, Relative}; return inst;
        case 0xF0: inst = (Instruction) {0xF0, "BEQ\0", 2, 2, Relative}; return inst;

        case 0x24: inst = (Instruction) {0x24, "BIT\0", 2, 3, ZeroPage}; return inst;
        case 0x2C: inst = (Instruction) {0x2C, "BIT\0", 3, 4, Absolute}; return inst;

        // Branch instructions part 2
        case 0x30: inst = (Instruction) {0x30, "BMI\0", 2, 2, Relative}; return inst;
        case 0xD0: inst = (Instruction) {0xD0, "BNE\0", 2, 2, Relative}; return inst;
        case 0x10: inst = (Instruction) {0x10, "BPL\0", 2, 2, Relative}; return inst;

        case 0x00: inst = (Instruction) {0x00, "BRK\0", 1, 7, Implied}; return inst;

        // Branch instructions part 3
        case 0x50: inst = (Instruction) {0x50, "BVC\0", 2, 2, Relative}; return inst;
        case 0x70: inst = (Instruction) {0x70, "BVS\0", 2, 2, Relative}; return inst;

        // Clear instructions
        case 0x18: inst = (Instruction) {0x18, "CLC\0", 1, 2, Implied}; return inst;
        case 0xD8: inst = (Instruction) {0xD8, "CLD\0", 1, 2, Implied}; return inst;
        case 0x58: inst = (Instruction) {0x58, "CLI\0", 1, 2, Implied}; return inst;
        case 0xB8: inst = (Instruction) {0xB8, "CLV\0", 1, 2, Implied}; return inst;

        // Compare instructions
        case 0xC9: inst = (Instruction) {0xC9, "CMP\0", 2, 2, Immediate}; return inst;
        case 0xC5: inst = (Instruction) {0xC5, "CMP\0", 2, 3, ZeroPage}; return inst;
        case 0xD5: inst = (Instruction) {0xD5, "CMP\0", 2, 4, ZeroPageX}; return inst;
        case 0xCD: inst = (Instruction) {0xCD, "CMP\0", 3, 4, Absolute}; return inst;
        case 0xDD: inst = (Instruction) {0xDD, "CMP\0", 3, 4, AbsoluteX}; return inst;
        case 0xD9: inst = (Instruction) {0xD9, "CMP\0", 3, 4, AbsoluteY}; return inst;
        case 0xC1: inst = (Instruction) {0xC1, "CMP\0", 2, 6, IndirectX}; return inst;
        case 0xD1: inst = (Instruction) {0xD1, "CMP\0", 2, 5, IndirectY}; return inst;

        case 0xE0: inst = (Instruction) {0xE0, "CPX\0", 2, 2, Immediate}; return inst;
        case 0xE4: inst = (Instruction) {0xE4, "CPX\0", 2, 3, ZeroPage}; return inst;
        case 0xEC: inst = (Instruction) {0xEC, "CPX\0", 3, 4, Absolute}; return inst;

        case 0xC0: inst = (Instruction) {0xC0, "CPY\0", 2, 2, Immediate}; return inst;
        case 0xC4: inst = (Instruction) {0xC4, "CPY\0", 2, 3, ZeroPage}; return inst;
        case 0xCC: inst = (Instruction) {0xCC, "CPY\0", 3, 4, Absolute}; return inst;

        // Decrement instructions
        case 0xC6: inst = (Instruction) {0xC6, "DEC\0", 2, 5, ZeroPage}; return inst;
        case 0xD6: inst = (Instruction) {0xD6, "DEC\0", 2, 6, ZeroPageX}; return inst;
        case 0xCE: inst = (Instruction) {0xCE, "DEC\0", 3, 6, Absolute}; return inst;
        case 0xDE: inst = (Instruction) {0xDE, "DEC\0", 3, 7, AbsoluteX}; return inst;

        case 0xCA: inst = (Instruction) {0xCA, "DEX\0", 1, 2, Implied}; return inst;
        case 0x88: inst = (Instruction) {0x88, "DEY\0", 1, 2, Implied}; return inst;

        case 0x49: inst = (Instruction) {0x49, "EOR\0", 2, 2, Immediate}; return inst;
        case 0x45: inst = (Instruction) {0x45, "EOR\0", 2, 3, ZeroPage}; return inst;
        case 0x55: inst = (Instruction) {0x55, "EOR\0", 2, 4, ZeroPageX}; return inst;
        case 0x4D: inst = (Instruction) {0x4D, "EOR\0", 3, 4, Absolute}; return inst;
        case 0x5D: inst = (Instruction) {0x5D, "EOR\0", 3, 4, AbsoluteX}; return inst;
        case 0x59: inst = (Instruction) {0x59, "EOR\0", 3, 4, AbsoluteY}; return inst;
        case 0x41: inst = (Instruction) {0x41, "EOR\0", 2, 6, IndirectX}; return inst;
        case 0x51: inst = (Instruction) {0x51, "EOR\0", 2, 5, IndirectY}; return inst;

        // Increment instructions
        case 0xE6: inst = (Instruction) {0xE6, "INC\0", 2, 5, ZeroPage}; return inst;
        case 0xF6: inst = (Instruction) {0xF6, "INC\0", 2, 6, ZeroPageX}; return inst;
        case 0xEE: inst = (Instruction) {0xEE, "INC\0", 3, 6, Absolute}; return inst;
        case 0xFE: inst = (Instruction) {0xFE, "INC\0", 3, 7, AbsoluteX}; return inst;

        case 0xE8: inst = (Instruction) {0xE8, "INX\0", 1, 2, Implied}; return inst;

        case 0xC8: inst = (Instruction) {0xC8, "INY\0", 1, 2, Implied}; return inst;

        // Jump instructions
        case 0x4C: inst = (Instruction) {0x4C, "JMP\0", 3, 3, Absolute}; return inst;
        case 0x6C: inst = (Instruction) {0x6C, "JMP\0", 3, 5, Indirect}; return inst;

        case 0x20: inst = (Instruction) {0x20, "JSR\0", 3, 6, Absolute}; return inst;

        // Load instructions
        case 0xA9: inst = (Instruction) {0xA9, "LDA\0", 2, 2, Immediate}; return inst;
        case 0xA5: inst = (Instruction) {0xA5, "LDA\0", 2, 3, ZeroPage}; return inst;
        case 0xB5: inst = (Instruction) {0xB5, "LDA\0", 2, 4, ZeroPageX}; return inst;
        case 0xAD: inst = (Instruction) {0xAD, "LDA\0", 3, 4, Absolute}; return inst;
        case 0xBD: inst = (Instruction) {0xBD, "LDA\0", 3, 4, AbsoluteX}; return inst;
        case 0xB9: inst = (Instruction) {0xB9, "LDA\0", 3, 4, AbsoluteY}; return inst;
        case 0xA1: inst = (Instruction) {0xA1, "LDA\0", 2, 6, IndirectX}; return inst;
        case 0xB1: inst = (Instruction) {0xB1, "LDA\0", 2, 5, IndirectY}; return inst;

        case 0xA2: inst = (Instruction) {0xA2, "LDX\0", 2, 2, Immediate}; return inst;
        case 0xA6: inst = (Instruction) {0xA6, "LDX\0", 2, 3, ZeroPage}; return inst;
        case 0xB6: inst = (Instruction) {0xB6, "LDX\0", 2, 4, ZeroPageY}; return inst;
        case 0xAE: inst = (Instruction) {0xAE, "LDX\0", 3, 4, Absolute}; return inst;
        case 0xBE: inst = (Instruction) {0xBE, "LDX\0", 3, 4, AbsoluteY}; return inst;

        case 0xA0: inst = (Instruction) {0xA0, "LDY\0", 2, 2, Immediate}; return inst;
        case 0xA4: inst = (Instruction) {0xA4, "LDY\0", 2, 3, ZeroPage}; return inst;
        case 0xB4: inst = (Instruction) {0xB4, "LDY\0", 2, 4, ZeroPageX}; return inst;
        case 0xAC: inst = (Instruction) {0xAC, "LDY\0", 3, 4, Absolute}; return inst;
        case 0xBC: inst = (Instruction) {0xBC, "LDY\0", 3, 4, AbsoluteX}; return inst;

        case 0x4A: inst = (Instruction) {0x4A, "LSR\0", 1, 2, Implied}; return inst;
        case 0x46: inst = (Instruction) {0x46, "LSR\0", 2, 5, ZeroPage}; return inst;
        case 0x56: inst = (Instruction) {0x56, "LSR\0", 2, 6, ZeroPageX}; return inst;
        case 0x4E: inst = (Instruction) {0x4E, "LSR\0", 3, 6, Absolute}; return inst;
        case 0x5E: inst = (Instruction) {0x5E, "LSR\0", 3, 7, AbsoluteX}; return inst;

        case 0xEA: inst = (Instruction) {0xEA, "NOP\0", 1, 2, Implied}; return inst;

        case 0x09: inst = (Instruction) {0x09, "ORA\0", 2, 2, Immediate}; return inst;
        case 0x05: inst = (Instruction) {0x05, "ORA\0", 2, 3, ZeroPage}; return inst;
        case 0x15: inst = (Instruction) {0x15, "ORA\0", 2, 4, ZeroPageX}; return inst;
        case 0x0D: inst = (Instruction) {0x0D, "ORA\0", 3, 4, Absolute}; return inst;
        case 0x1D: inst = (Instruction) {0x1D, "ORA\0", 3, 4, AbsoluteX}; return inst;
        case 0x19: inst = (Instruction) {0x19, "ORA\0", 3, 4, AbsoluteY}; return inst;
        case 0x01: inst = (Instruction) {0x01, "ORA\0", 2, 6, IndirectX}; return inst;
        case 0x11: inst = (Instruction) {0x11, "ORA\0", 2, 5, IndirectY}; return inst;

        // Push instructions
        case 0x48: inst = (Instruction) {0x48, "PHA\0", 1, 3, Implied}; return inst;
        case 0x08: inst = (Instruction) {0x08, "PHP\0", 1, 3, Implied}; return inst;

        // Pull instructions
        case 0x68: inst = (Instruction) {0x68, "PLA\0", 1, 4, Implied}; return inst;
        case 0x28: inst = (Instruction) {0x28, "PLP\0", 1, 4, Implied}; return inst;

        // Rotate instructions
        case 0x2A: inst = (Instruction) {0x2A, "ROL\0", 1, 2, Implied}; return inst;
        case 0x26: inst = (Instruction) {0x26, "ROL\0", 2, 5, ZeroPage}; return inst;
        case 0x36: inst = (Instruction) {0x36, "ROL\0", 2, 6, ZeroPageX}; return inst;
        case 0x2E: inst = (Instruction) {0x2E, "ROL\0", 3, 6, Absolute}; return inst;
        case 0x3E: inst = (Instruction) {0x3E, "ROL\0", 3, 7, AbsoluteX}; return inst;

        case 0x6A: inst = (Instruction) {0x6A, "ROR\0", 1, 2, Implied}; return inst;
        case 0x66: inst = (Instruction) {0x66, "ROR\0", 2, 5, ZeroPage}; return inst;
        case 0x76: inst = (Instruction) {0x76, "ROR\0", 2, 6, ZeroPageX}; return inst;
        case 0x6E: inst = (Instruction) {0x6E, "ROR\0", 3, 6, Absolute}; return inst;
        case 0x7E: inst = (Instruction) {0x7E, "ROR\0", 3, 7, AbsoluteX}; return inst;

        // Return instructions
        case 0x40: inst = (Instruction) {0x40, "RTI\0", 1, 6, Implied}; return inst;
        case 0x60: inst = (Instruction) {0x60, "RTS\0", 1, 6, Implied}; return inst;

        case 0xE9: inst = (Instruction) {0xE9, "SBC\0", 2, 2, Immediate}; return inst;
        case 0xE5: inst = (Instruction) {0xE5, "SBC\0", 2, 3, ZeroPage}; return inst;
        case 0xF5: inst = (Instruction) {0xF5, "SBC\0", 2, 4, ZeroPageX}; return inst;
        case 0xED: inst = (Instruction) {0xED, "SBC\0", 3, 4, Absolute}; return inst;
        case 0xFD: inst = (Instruction) {0xFD, "SBC\0", 3, 4, AbsoluteX}; return inst;
        case 0xF9: inst = (Instruction) {0xF9, "SBC\0", 3, 4, AbsoluteY}; return inst;
        case 0xE1: inst = (Instruction) {0xE1, "SBC\0", 2, 6, IndirectX}; return inst;
        case 0xF1: inst = (Instruction) {0xF1, "SBC\0", 2, 5, IndirectY}; return inst;

        // Set flag intructions
        case 0x38: inst = (Instruction) {0x38, "SEC\0", 1, 2, Implied}; return inst;
        case 0xF8: inst = (Instruction) {0xF8, "SED\0", 1, 2, Implied}; return inst;
        case 0x78: inst = (Instruction) {0x78, "SEI\0", 1, 2, Implied}; return inst;

        // Store instructions
        case 0x85: inst = (Instruction) {0x85, "STA\0", 2, 3, ZeroPage}; return inst;
        case 0x95: inst = (Instruction) {0x95, "STA\0", 2, 4, ZeroPageX}; return inst;
        case 0x8D: inst = (Instruction) {0x8D, "STA\0", 3, 4, Absolute}; return inst;
        case 0x9D: inst = (Instruction) {0x9D, "STA\0", 3, 5, AbsoluteX}; return inst;
        case 0x99: inst = (Instruction) {0x99, "STA\0", 3, 5, AbsoluteY}; return inst;
        case 0x81: inst = (Instruction) {0x81, "STA\0", 2, 6, IndirectX}; return inst;
        case 0x91: inst = (Instruction) {0x91, "STA\0", 2, 6, IndirectY}; return inst;

        case 0x86: inst = (Instruction) {0x86, "STX\0", 2, 3, ZeroPage}; return inst;
        case 0x96: inst = (Instruction) {0x96, "STX\0", 2, 4, ZeroPageY}; return inst;
        case 0x8E: inst = (Instruction) {0x8E, "STX\0", 3, 4, Absolute}; return inst;

        case 0x84: inst = (Instruction) {0x84, "STY\0", 2, 3, ZeroPage}; return inst;
        case 0x94: inst = (Instruction) {0x94, "STY\0", 2, 4, ZeroPageX}; return inst;
        case 0x8C: inst = (Instruction) {0x8C, "STY\0", 3, 4, Absolute}; return inst;
        
        // Transfer instructions
        case 0xAA: inst = (Instruction) {0xAA, "TAX\0", 1, 2, Implied}; return inst;
        case 0xA8: inst = (Instruction) {0xA8, "TAY\0", 1, 2, Implied}; return inst;
        case 0xBA: inst = (Instruction) {0xBA, "TSX\0", 1, 2, Implied}; return inst;
        case 0x8A: inst = (Instruction) {0x8A, "TXA\0", 1, 2, Implied}; return inst;
        case 0x9A: inst = (Instruction) {0x9A, "TXS\0", 1, 2, Implied}; return inst;
        case 0x98: inst = (Instruction) {0x98, "TYA\0", 1, 2, Implied}; return inst;

        default: inst = (Instruction) {0x00, "BRK\0", 1, 7, Implied}; return inst;
    }
}

// Instructions

void adc(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = mem_read(cpu, addr);
    add_with_carry(cpu, operand);
}

void and(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = mem_read(cpu, addr);
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
    uint8_t value = mem_read(cpu, addr);
    mem_write(cpu, value, addr);
    update_carry_flag(cpu, value);
}

// Branch instructions part 1

void bcc(CPU *cpu)
{
    branch(cpu, !is_set(cpu, CARRY_FLAG));
}

void bcs(CPU *cpu)
{
    branch(cpu, is_set(cpu, CARRY_FLAG));
}

void beq(CPU *cpu)
{
    branch(cpu, is_set(cpu, ZERO_FLAG));
}

void bit(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = mem_read(cpu, addr);

    ((cpu->reg_a & operand) == 0) ? set_flag(cpu, ZERO_FLAG) : unset_flag(cpu, ZERO_FLAG);
    
    // Set V flag to operand 6th bit
    if ((operand & 0b01000000) != 0)
    {
        set_flag(cpu, OVERFLOW_FLAG);
    }
    else
    {
        unset_flag(cpu, OVERFLOW_FLAG);
    }

    // Set N flag to operand 7th bit
    if ((operand & 0b10000000) != 0)
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
    branch(cpu, is_set(cpu, NEGATIVE_FLAG));
}

void bne(CPU *cpu)
{
    branch(cpu, !is_set(cpu, ZERO_FLAG));
}

void bpl(CPU *cpu)
{
    branch(cpu, !is_set(cpu, NEGATIVE_FLAG));
}

void brk(CPU *cpu)
{
    return;
}

// Branch instructions part 3

void bvc(CPU *cpu)
{
    branch(cpu, !is_set(cpu, OVERFLOW_FLAG));
}

void bvs(CPU *cpu)
{
    branch(cpu, is_set(cpu, OVERFLOW_FLAG));
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
    uint8_t operand = mem_read(cpu, addr);
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
    uint8_t operand = mem_read(cpu, addr);
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
    uint8_t operand = mem_read(cpu, addr);
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
    uint8_t operand = mem_read(cpu, addr);
    operand--;
    mem_write(cpu, operand, addr);
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
    uint8_t operand = mem_read(cpu, addr);
    cpu->reg_a ^= operand;
    update_zero_and_negative_flags(cpu, cpu->reg_a); 
}

// Increment instruction

void inc(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = mem_read(cpu, addr);
    operand++;
    mem_write(cpu, operand, addr);
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
    uint16_t addr = mem_read_u16(cpu, cpu->program_counter);
    cpu->program_counter = addr;
}

// Load instructions

void lda(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t value = mem_read(cpu, addr);
    set_reg_a(cpu, value);
}

void ldx(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = mem_read(cpu, addr);
    set_reg_x(cpu, operand);
}

void ldy(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = mem_read(cpu, addr);
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
    uint8_t operand = mem_read(cpu, addr);
    if ((operand & 1) != 0)
    {
        set_flag(cpu, CARRY_FLAG);
    }
    else
    {
        unset_flag(cpu, CARRY_FLAG);
    }
    mem_write(cpu, operand >> 1, addr);
    update_zero_and_negative_flags(cpu, operand >> 1);
}

void ora(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    uint8_t operand = mem_read(cpu, addr);
    set_reg_a(cpu, cpu->reg_a | operand);
}

// Push instructions

void pha(CPU *cpu)
{
    stack_push(cpu, cpu->reg_a);
}

void php(CPU *cpu)
{
    uint8_t old_status_register = cpu->status;
    set_flag(cpu, BREAK_FLAG_0);
    set_flag(cpu, BREAK_FLAG_1);
    stack_push(cpu, cpu->status);
    cpu->status = old_status_register;
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
    unset_flag(cpu, BREAK_FLAG_0); // This flag is ignored by this instruction
    set_flag(cpu, BREAK_FLAG_1); // This flag must always be set
}

// Rotate instructions

void rol_acc(CPU *cpu)
{
    uint8_t old_acc = cpu->reg_a;
    set_reg_a(cpu, cpu->reg_a << 1);
    if (is_set(cpu, CARRY_FLAG))
    {
        set_reg_a(cpu, cpu->reg_a | 0x01);
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
    uint8_t operand = mem_read(cpu, addr);
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
    mem_write(cpu, new_value, addr);
    update_zero_and_negative_flags(cpu, new_value);
}

void ror_acc(CPU *cpu)
{
    uint8_t old_acc = cpu->reg_a;
    set_reg_a(cpu, cpu->reg_a >> 1);
    if (is_set(cpu, CARRY_FLAG))
    {
        set_reg_a(cpu, cpu->reg_a | 0x80);
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
    uint8_t operand = mem_read(cpu, addr);
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
    mem_write(cpu, new_value, addr);
    update_zero_and_negative_flags(cpu, new_value);
}

// Return instructions

void rti(CPU *cpu)
{
    uint8_t flags = stack_pull(cpu);
    uint16_t program_counter = stack_pull_u16(cpu);
    cpu->status = flags;
    // "Disregards" bits 5 and 4, which means 5 is set and 4 isn't
    set_flag(cpu, BREAK_FLAG_1);
    unset_flag(cpu, BREAK_FLAG_0);
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
    uint8_t operand = mem_read(cpu, addr);
    // Must be the one's complement, not the two's
    add_with_carry(cpu, ~operand);
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
    mem_write(cpu, cpu->reg_a, addr);
}

void stx(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    mem_write(cpu, cpu->reg_x, addr);
}

void sty(CPU *cpu, AddrMode mode)
{
    uint16_t addr = get_operand_addr(cpu, mode);
    mem_write(cpu, cpu->reg_y, addr);
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

void branch(CPU *cpu, bool condition)
{
    cpu->program_counter += (condition) ? (int8_t) mem_read(cpu, cpu->program_counter) : 0;
    cpu->program_counter++;
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

        case ZeroPage: return mem_read(cpu, cpu->program_counter);
        case ZeroPageX: return mem_read(cpu, cpu->program_counter) + (int8_t) cpu->reg_x;
        case ZeroPageY: return mem_read(cpu, cpu->program_counter) + (int8_t) cpu->reg_y;

        case Absolute: return mem_read_u16(cpu, cpu->program_counter);
        case AbsoluteX: return mem_read_u16(cpu, cpu->program_counter) + (int8_t) cpu->reg_x;
        case AbsoluteY: return mem_read_u16(cpu, cpu->program_counter) + (int8_t) cpu->reg_y;

        case Indirect:
            uint16_t base_u16 = mem_read_u16(cpu, cpu->program_counter);
            return mem_read_u16(cpu, base_u16);
        case IndirectX:
            uint8_t base_x = mem_read(cpu, cpu->program_counter);
            return mem_read_u16(cpu, base_x + cpu->reg_x);
        case IndirectY:
            uint8_t base_y = mem_read(cpu, cpu->program_counter);
            return mem_read_u16(cpu, base_y) + cpu->reg_y;
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