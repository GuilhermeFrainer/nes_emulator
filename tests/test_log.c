#include "test_framework.h"
#include "../lib/cpu.h"
#include "../lib/cartridge.h"
#include "../lib/instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NES_TEST_PATH "cartridges/nestest.nes"

#define PROGRAM_COUNTER_LENGTH 4
#define HEX_OPCODE_LENGTH 8
#define ASSEMBLY_OPCODE_LENGTH 30
#define CPU_REGS_LENGTH 25
#define FINAL_SIZE PROGRAM_COUNTER_LENGTH + HEX_OPCODE_LENGTH \
        + ASSEMBLY_OPCODE_LENGTH + CPU_REGS_LENGTH + 7 // 6 spaces + \n char


typedef struct PrintString {
    char program_counter[PROGRAM_COUNTER_LENGTH + 1];
    char hex_opcode[HEX_OPCODE_LENGTH + 1];
    char assembly_opcode[ASSEMBLY_OPCODE_LENGTH + 1];
    char cpu_regs[CPU_REGS_LENGTH + 1];
    // TODO:
    // char clock_cycles[];
} PrintString;

PrintString new_print_string(CPU *cpu, Instruction inst, uint16_t original_pc);
char *get_assembly_opcode(CPU *cpu, Instruction inst, uint16_t original_pc);
void run_and_log(CPU *cpu);
void enter_log(CPU *cpu, FILE *file, Instruction inst, uint16_t original_pc);

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

PrintString new_print_string(CPU *cpu, Instruction inst, uint16_t original_pc)
{
    PrintString p_string;

    // Initialize all strings
    memset(p_string.program_counter, ' ', PROGRAM_COUNTER_LENGTH);
    memset(p_string.hex_opcode, ' ', HEX_OPCODE_LENGTH);
    memset(p_string.assembly_opcode, ' ', ASSEMBLY_OPCODE_LENGTH);
    memset(p_string.cpu_regs, ' ', CPU_REGS_LENGTH);

    // Assign null characters at the end of every string
    p_string.program_counter[PROGRAM_COUNTER_LENGTH] = '\0';
    p_string.hex_opcode[HEX_OPCODE_LENGTH] = '\0';
    p_string.assembly_opcode[ASSEMBLY_OPCODE_LENGTH] = '\0';
    p_string.cpu_regs[CPU_REGS_LENGTH] = '\0';

    sprintf(p_string.program_counter, "%4X", cpu->program_counter);

    // Works fine up to here

    switch (inst.bytes)
    {
    case 2:
        sprintf(
            p_string.hex_opcode,
            "%02X %02X   ",
            inst.opcode,
            mem_read(cpu, cpu->program_counter + 1)
        );
        break;
    
    case 3:
        sprintf(
            p_string.hex_opcode,
            "%02X %02X %02X",
            inst.opcode,
            mem_read(cpu, cpu->program_counter + 1),
            mem_read(cpu, cpu->program_counter + 2)
        );
        break;

    default:
        sprintf(
            p_string.hex_opcode,
            "%02X      ",
            inst.opcode
        );
        break;
    }

    char *assembly_opcode = get_assembly_opcode(cpu, inst, original_pc);
    if (assembly_opcode == NULL)
    {
        fprintf(stderr, "Something went wrong when trying to get the assembly opcode.\n");
    }

    strcpy(p_string.assembly_opcode, assembly_opcode);
    free(assembly_opcode);

    sprintf(
        p_string.cpu_regs,
        "A:%02X X:%02X Y:%02X P:%02X SP:%02X",
        cpu->reg_a,
        cpu->reg_x,
        cpu->reg_y,
        cpu->status,
        cpu->stack_pointer
    );

    return p_string;
}

char *get_assembly_opcode(CPU *cpu, Instruction inst, uint16_t original_pc)
{
    // Done so 'get_operand_addr()' runs as expected
    cpu->program_counter++;
    uint16_t addr = get_operand_addr(cpu, inst.mode);
    
    uint8_t operand;
    char *return_string  = malloc(sizeof(char) * (ASSEMBLY_OPCODE_LENGTH + 1));
    if (return_string == NULL)
    {
        return NULL;
    }

    switch (inst.mode)
    {
        case Immediate:
            sprintf(
                return_string,
                "%s #$%02X",
                inst.mnemonic,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPage:
            sprintf(
                return_string,
                "%s $%02X = $%02X",
                inst.mnemonic,
                addr,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPageX:
            sprintf(
                return_string,
                "%s $%02X,X @ %20X = %02X",
                inst.mnemonic,
                mem_read(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case ZeroPageY:
            sprintf(
                return_string,
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
                    return_string,
                    "%s $%04X",
                    inst.mnemonic,
                    addr
                );
            }
            else
            {
                sprintf(
                    return_string,
                    "%s $%04X = %02X",
                    inst.mnemonic,
                    addr,
                    mem_read(cpu, addr)
                );
            }
            break;

        case AbsoluteX:
            sprintf(
                return_string,
                "%s $%04X,X @ %04X = %02X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case AbsoluteY:
            sprintf(
                return_string,
                "%s $%04X,Y @ %04X = %02X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case Indirect:
            sprintf(
                return_string,
                "%s ($%04X) = %04X",
                inst.mnemonic,
                mem_read_u16(cpu, cpu->program_counter),
                addr
            );
            break;

        case IndirectX:
            operand = mem_read(cpu, cpu->program_counter);
            sprintf(
                return_string,
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
                return_string,
                "%s ($%02X),Y = %04X @ %04X = %02X",
                inst.mnemonic,
                operand,
                mem_read_u16(cpu, operand),
                addr,
                mem_read(cpu, addr)
            );
            break;

        case Implied:
            sprintf(return_string, "%s", inst.mnemonic);
            break;
    }
    // Compensate previous incrementation
    cpu->program_counter--;

    int length = strlen(return_string);
    memset(&return_string[length], ' ', ASSEMBLY_OPCODE_LENGTH - length);
    
    // Fill the remaining spaces with space characters and null-terminate the string
    //for (int i = strlen(return_string); i < sizeof(return_string) - 1; i++)
    //{
    //    return_string[i] = 'a';
    //}
    return_string[ASSEMBLY_OPCODE_LENGTH] = '\0';

    return return_string;
}

void run_and_log(CPU *cpu)
{
    FILE *file = fopen("my_log.log", "w");
    if (file == NULL)
    {
        fprintf(stderr, "Something went wrong while creating a file");
        return;
    }

    while (1)
    {
        uint8_t opcode = mem_read(cpu, cpu->program_counter);
        uint16_t original_pc = cpu->program_counter;
        Instruction inst = inst_list[opcode];
        
        enter_log(cpu, file, inst, original_pc);
        interpret(cpu, opcode);
        if (opcode == 0x00)
        {
            break;
        }
    }

    fclose(file);
}

void enter_log(CPU *cpu, FILE *file, Instruction inst, uint16_t original_pc)
{
    PrintString p_string = new_print_string(cpu, inst, original_pc);
    char string_to_write[FINAL_SIZE + 1];
    sprintf(
        string_to_write,
        "%s  %s  %s  %s\n", // Two spaces between each string
        p_string.program_counter,
        p_string.hex_opcode,
        p_string.assembly_opcode,
        p_string.cpu_regs
    );
    fwrite(string_to_write, strlen(string_to_write), 1, file);
}