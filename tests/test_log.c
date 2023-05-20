#include "test_framework.h"
#include "../lib/cpu.h"
#include "../lib/cartridge.h"
#include "../lib/instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NES_TEST_PATH "cartridges/nestest.nes"

#define LINE_LENGTH 75 // Takes the \n and \0 characters into account
#define HEX_OPCODE_POSITION 4
#define ASSEMBLY_OPCODE_POSITION 12
#define CPU_REGS_POSITION 42

void write_line_string(char *line_string, CPU *cpu, Instruction inst);
void write_assembly_opcode(char *line_string, CPU *cpu, Instruction inst);
void run_and_log(CPU *cpu);
void enter_log(CPU *cpu, FILE *file, Instruction inst);

int main(int argc, char **argv)
{
    char *file_path = NES_TEST_PATH;
    ROM *rom = get_rom(file_path);
    if (rom == NULL)
    {
        return 1;
    }
    CPU *cpu = new_cpu(rom);
    populate_inst_list();
    //load(cpu);
    reset(cpu);
    cpu->program_counter = 0xC000;

    run_and_log(cpu);

    destroy_cpu(cpu);
    return 0;
}

void write_line_string(char *line_string, CPU *cpu, Instruction inst)
{
    // Initializes the string
    memset(line_string, ' ', LINE_LENGTH);
    line_string[LINE_LENGTH - 1] = '\0';

    // Writes the program counter
    sprintf(line_string, "%4X", cpu->program_counter);

    // Writes the hex opcode
    switch (inst.bytes)
    {
    case 2:
        sprintf(
            &line_string[HEX_OPCODE_POSITION],
            "%02X %02X   ",
            inst.opcode,
            mem_read(cpu, cpu->program_counter + 1)
        );
        break;
    
    case 3:
        sprintf(
            &line_string[HEX_OPCODE_POSITION],
            "%02X %02X %02X",
            inst.opcode,
            mem_read(cpu, cpu->program_counter + 1),
            mem_read(cpu, cpu->program_counter + 2)
        );
        break;

    default:
        sprintf(
            &line_string[HEX_OPCODE_POSITION],
            "%02X      ",
            inst.opcode
        );
        break;
    }

    // Writes the assembly opcode
    // Must increment and then decrement the PC for it to work properly
    cpu->program_counter++;
    write_assembly_opcode(line_string, cpu, inst);
    cpu->program_counter--;

    // Writes the cpu registers
    sprintf(
        &line_string[CPU_REGS_POSITION],
        "A:%02X X:%02X Y:%02X P:%02X SP:%02X",
        cpu->reg_a,
        cpu->reg_x,
        cpu->reg_y,
        cpu->status,
        cpu->stack_pointer
    );
}

void write_assembly_opcode(char *line_string, CPU *cpu, Instruction inst)
{
    uint16_t addr = get_operand_addr(cpu, inst.mode);

    uint8_t operand;

    switch (inst.mode)
    {
        case Immediate:
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
                "%s #$%02X",
                inst.mnemonic,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPage:
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
                "%s $%02X = $%02X",
                inst.mnemonic,
                addr,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPageX:
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
                "%s $%02X,X @ %20X = %02X",
                inst.mnemonic,
                mem_read(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPageY:
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
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
                sprintf(
                    &line_string[ASSEMBLY_OPCODE_POSITION],
                    "%s $%04X",
                    inst.mnemonic,
                    addr
                );
            }
            else
            {
                sprintf(
                    &line_string[ASSEMBLY_OPCODE_POSITION],
                    "%s $%04X = %02X",
                    inst.mnemonic,
                    addr,
                    mem_read(cpu, addr)
                );
            }
            break;

        case AbsoluteX:
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
                "%s $%04X,X @ %04X = %02X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case AbsoluteY:
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
                "%s $%04X,Y @ %04X = %02X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case Indirect:
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
                "%s ($%04X) = %04X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr
            );
            break;

        case IndirectX:
            operand = mem_read(cpu, cpu->program_counter);
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
                "%s ($%02X,X) @ %02X = %04X = %02X",
                inst.mnemonic,
                operand,
                (int8_t) operand + cpu->reg_x,
                addr,
                mem_read(cpu, addr)
            );
            break;

        case IndirectY:
            operand = mem_read(cpu, cpu->program_counter);
            sprintf(
                &line_string[ASSEMBLY_OPCODE_POSITION],
                "%s ($%02X),Y = %04X @ %04X = %02X",
                inst.mnemonic,
                operand,
                mem_read_u16(cpu, operand),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case Implied:
            sprintf(&line_string[ASSEMBLY_OPCODE_POSITION], "%s", inst.mnemonic);
            break;
    }

    int assembly_opcode_start = strlen(&line_string[ASSEMBLY_OPCODE_POSITION]);

    // Fills the rest of the assembly opcode area with spaces
    for (int i = assembly_opcode_start; i < CPU_REGS_POSITION; i++)
    {
        line_string[i] = ' ';
    }
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
    fwrite(string_to_write, strlen(string_to_write), 1, file);
    fflush(file);
}