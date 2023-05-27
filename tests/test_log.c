#include "test_framework.h"
#include "../lib/cpu.h"
#include "../lib/cartridge.h"
#include "../lib/instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NES_TEST_PATH "cartridges/nestest.nes"

#define LINE_LENGTH 75
#define CPU_REGS_POSITION 48

void write_line_string(char *line_string, CPU *cpu, Instruction inst);
void run_and_log(CPU *cpu);
void enter_log(CPU *cpu, FILE *file, Instruction inst);

int main(int argc, char **argv)
{
    ROM *rom = get_rom(NES_TEST_PATH);
    if (rom == NULL)
    {
        return 1;
    }
    CPU *cpu = new_cpu(rom);
    populate_inst_list();
    reset(cpu);
    cpu->program_counter = 0xC000;

    run_and_log(cpu);

    destroy_cpu(cpu);
    return 0;
}

void write_line_string(char *line_string, CPU *cpu, Instruction inst)
{
    // Pointer to keep track of the last character
    int last_char_position = 0;

    // Handles the program counter
    last_char_position += sprintf(
        line_string,
        "%04X  ",
        cpu->program_counter
    );

    // Handles the hex opcode
    switch (inst.bytes)
    {
        case 3:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%02X %02X %02X  ",
                inst.opcode,
                mem_read(cpu, cpu->program_counter + 1),
                mem_read(cpu, cpu->program_counter + 2)
            );
            break;
    
        case 2:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%02X %02X     ",
                inst.opcode,
                mem_read(cpu, cpu->program_counter + 1)
            );
            break;

        default:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%02X        ",
                inst.opcode
            );
            break;
    }

    // Handles the assembly opcode
    // Must increment the program counter and decrement it later for this to work properly
    cpu->program_counter++;
    uint16_t addr = get_operand_addr(cpu, inst.mode);
    cpu->program_counter--;

    uint8_t operand;

    switch (inst.mode)
    {
        case Immediate:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s #$%02X",
                inst.mnemonic,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPage:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s $%02X = %02X",
                inst.mnemonic,
                addr,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPageX:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s $%02X,X @ %20X = %02X",
                inst.mnemonic,
                mem_read(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPageY:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s $%02X,Y @ %02X = %02X",
                inst.mnemonic,
                mem_read(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case Absolute:
            if (inst.opcode == 0x4C || inst.opcode == 0x20)
            {
                last_char_position += sprintf(
                    line_string + last_char_position,
                    "%s $%04X",
                    inst.mnemonic,
                    addr
                );
            }
            else
            {
                last_char_position += sprintf(
                    line_string + last_char_position,
                    "%s $%04X = %02X",
                    inst.mnemonic,
                    addr,
                    mem_read(cpu, addr)
                );
            }
            break;

        case AbsoluteX:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s $%04X,X @ %04X = %02X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case AbsoluteY:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s $%04X,Y @ %04X = %02X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case Indirect:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s ($%04X) = %04X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr
            );
            break;

        case IndirectX:
            operand = mem_read(cpu, cpu->program_counter + 1);
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s ($%02X,X) @ %02X = %04X = %02X",
                inst.mnemonic,
                operand,
                operand + cpu->reg_x,
                addr,
                mem_read(cpu, addr)
            );
            break;

        case IndirectY:
            operand = mem_read(cpu, cpu->program_counter + 1);
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s ($%02X),Y = %04X @ %04X = %02X",
                inst.mnemonic,
                operand,
                mem_read_u16(cpu, operand),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case Implied:
            switch (inst.opcode)
            {
                // Handles accumulator instructions
                case 0x0A:
                case 0x4A:
                case 0x2A:
                case 0x6A:
                    last_char_position += sprintf(
                        line_string + last_char_position,
                        "%s A",
                        inst.mnemonic
                    );
                    break;
                
                default:
                    last_char_position += sprintf(
                        line_string + last_char_position,
                        "%s",
                        inst.mnemonic
                    );
                    break;
            }
            break;

        case Relative:
            last_char_position += sprintf(
                line_string + last_char_position,
                "%s $%04X",
                inst.mnemonic,
                cpu->program_counter + mem_read(cpu, cpu->program_counter + 1) + inst.bytes
            );
            break;
    }


    // Fills the rest of the assembly opcode area with spaces
    for (; last_char_position < CPU_REGS_POSITION; last_char_position++)
    {
        line_string[last_char_position] = ' ';
    }

    // Handles the CPU registers
    last_char_position += sprintf(
        line_string + last_char_position,
        "A:%02X X:%02X Y:%02X P:%02X SP:%02X",
        cpu->reg_a,
        cpu->reg_x,
        cpu->reg_y,
        cpu->status,
        cpu->stack_pointer
    );

    sprintf(
        line_string + last_char_position,
        "\n"
    );

    return;
}

void run_and_log(CPU *cpu)
{
    FILE *file = fopen("tests/my_log.log", "w");
    if (file == NULL)
    {
        fprintf(stderr, "Something went wrong while creating a file\n");
        return;
    }

    while (1)
    {
        uint8_t opcode = mem_read(cpu, cpu->program_counter);
        Instruction inst = inst_list[opcode];
        
        enter_log(cpu, file, inst);
        interpret(cpu, opcode);
        if (opcode == 0x00)
        {
            printf("Breaking at %X\n", cpu->program_counter);
            break;
        }
    }

    fclose(file);
}

void enter_log(CPU *cpu, FILE *file, Instruction inst)
{
    char string_to_write[LINE_LENGTH];
    write_line_string(string_to_write, cpu, inst);
    fwrite(string_to_write, strlen(string_to_write) * sizeof(char), 1, file);
}