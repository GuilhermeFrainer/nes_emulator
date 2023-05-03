#include "test_framework.h"
#include "../lib/instructions.h"
#include "../lib/cpu.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

int successful_tests = 0;
int failed_tests = 0;

void test_get_instruction_from_opcode(void);
void test_populate_inst_list(void);
void test_update_zero_and_negative_flags(void);
void test_get_operand_addr(void);
void test_instructions(void);
void test_add_with_carry(void);
void test_inc_instructions(void);
void test_loops(void);

int main(int argc, char **argv)
{
    test_get_instruction_from_opcode();
    test_populate_inst_list();
    test_update_zero_and_negative_flags();
    test_get_operand_addr();
    test_instructions();
    test_add_with_carry();
    test_inc_instructions();
    test_loops();
    end_tests();
}

void test_get_instruction_from_opcode(void)
{
    Instruction inst = get_instruction_from_opcode(0xA9);
    assert_eq(inst.opcode, 0xA9);
    assert_eq(inst.bytes, 2);
}

void test_populate_inst_list(void)
{
    populate_inst_list();
    Instruction brk = inst_list[0x00];
    assert_eq(brk.opcode, 0x00);
    Instruction lda = inst_list[0xA9];
    assert_eq(lda.opcode, 0xA9);
    assert_eq(lda.mode, Immediate);
}

void test_update_zero_and_negative_flags(void)
{
    CPU *cpu = new_cpu();
    update_zero_and_negative_flags(cpu, 0);
    assert_eq(cpu->status, 0b00000010);
    update_zero_and_negative_flags(cpu, 0b10110010);
    assert_eq(cpu->status, 0b10000000);
    update_zero_and_negative_flags(cpu, 0b00101101);
    assert_eq(cpu->status, 0b00000000);
    free(cpu);
}

void test_get_operand_addr(void)
{
    CPU *cpu = new_cpu();
    cpu->program_counter = 15;
    assert_eq(get_operand_addr(cpu, Immediate), 15);
    
    // Zero page
    cpu->reg_x = 5;
    cpu->reg_y = 10;
    mem_write(cpu, 0xAB, cpu->program_counter);
    assert_eq(get_operand_addr(cpu, ZeroPage), 0xAB);
    assert_eq(get_operand_addr(cpu, ZeroPageX), 0xAB + 5);
    assert_eq(get_operand_addr(cpu, ZeroPageY), 0xAB + 10);

    // Absolute
    cpu->program_counter = 100;
    cpu->reg_x = 50;
    cpu->reg_y = 25;
    mem_write_u16(cpu, 0xBABA, cpu->program_counter);
    assert_eq(get_operand_addr(cpu, Absolute), 0xBABA);
    assert_eq(get_operand_addr(cpu, AbsoluteX), 0xBABA + 50);
    assert_eq(get_operand_addr(cpu, AbsoluteY), 0xBABA + 25);

    // Indirect
    cpu->program_counter = 200;
    cpu->reg_x = 100;
    cpu->reg_y = 20;

    mem_write_u16(cpu, 0x0120, cpu->program_counter);
    mem_write_u16(cpu, 0xBAFC, 0x0120);
    assert_eq(get_operand_addr(cpu, Indirect), 0xBAFC);

    cpu->program_counter = 300;
    mem_write(cpu, 0xBC, cpu->program_counter);
    mem_write_u16(cpu, 0xABBA, 0xBC + cpu->reg_x);
    assert_eq(get_operand_addr(cpu, IndirectX), 0xABBA);

    mem_write_u16(cpu, 0xBAAB, 0xBC);
    assert_eq(get_operand_addr(cpu, IndirectY), 0xBAAB + cpu->reg_y);
    free(cpu);
}

void test_instructions(void)
{
    CPU *cpu = new_cpu();
    populate_inst_list();

    cpu->reg_a = 0x80;
    mem_write(cpu, 0x80, 0);
    adc(cpu, Immediate);
    assert_eq(cpu->status, CARRY_FLAG | OVERFLOW_FLAG);
    free(cpu);
}

// Tests the add with carry function and its instructions
void test_add_with_carry(void)
{
    // Initialization
    CPU *cpu = new_cpu();
    populate_inst_list();
    
    // Makes ADC operation that should overflow
    set_reg_a(cpu, 150);
    add_with_carry(cpu, 150, true);
    assert_eq(is_set(cpu, OVERFLOW_FLAG), true);
    assert_eq(is_set(cpu, CARRY_FLAG), true);

    // Cleanup
    free(cpu);
}

void test_inc_instructions(void)
{
    CPU *cpu = new_cpu();
    reset(cpu);
    mem_write(cpu, 0x10, cpu->program_counter);
    inc(cpu, Immediate);
    assert_eq(mem_read(cpu, cpu->program_counter), 0x11);

    free(cpu);
}

void test_loops(void)
{
    CPU *cpu = new_cpu();
    populate_inst_list();
    uint8_t program[] = {
        0xa2, 0x00, 0xa0, 0x00, 0x8a, 0x99, 0x00, 0x02, 0x48, 0xe8, 0xc8, 0xc0, 0x10, 0xd0, 0xf5, 0x68,
        0x99, 0x00, 0x02, 0xc8, 0xc0, 0x20, 0xd0, 0xf7
    };

    int program_length = sizeof(program)/sizeof(program[0]);
    
    load(cpu, program, program_length);
    reset(cpu);
    run_for_testing(cpu);
    assert_eq(cpu->reg_y, 0x20);

    free(cpu);
}