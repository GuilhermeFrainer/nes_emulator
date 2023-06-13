#include "test_framework.h"
#include "../lib/cpu.h"
#include "../lib/instructions.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void test_new(void);
void test_read_and_mem_write(void);
void test_reset(void);
void test_load(void);
void test_run(void);
void test_set_unset_flag(void);
void test_is_set(void);
void test_get_stack_addr(void);
void test_stack(void);

int successful_tests = 0;
int failed_tests = 0;

int main(int argc, char **argv)
{
    test_new();
    test_read_and_mem_write();
    test_reset();
    test_load();
    test_run();
    test_set_unset_flag();
    test_is_set();
    test_get_stack_addr();
    test_stack();
    end_tests();
}

void test_new(void)
{
    CPU *cpu = new_cpu();
    assert_eq(cpu->status, 0);
    assert_eq(cpu->reg_a, 0);
    assert_eq(cpu->reg_x, 0);
    assert_eq(cpu->reg_y, 0);
    assert_eq(cpu->program_counter, 0);
    free(cpu);
}

void test_read_and_mem_write(void)
{
    CPU *cpu = new_cpu();
    mem_write(cpu, 0xAB, 0x8000);
    assert_eq(cpu->memory[0x8000], 0xAB);
    assert_eq(mem_read(cpu, 0x8000), 0xAB);
    
    // 16 bit tests
    mem_write_u16(cpu, 0xABCD, 0x8000);
    assert_eq(cpu->memory[0x8000], 0xCD);
    assert_eq(cpu->memory[0x8000 + 1], 0xAB);
    assert_eq(mem_read_u16(cpu, 0x8000), 0xABCD);
    free(cpu);
}

void test_reset(void)
{
    CPU *cpu = new_cpu();
    cpu->reg_a = 10;
    cpu->reg_x = 25;
    cpu->reg_y = 50;
    cpu->status = 64;
    mem_write_u16(cpu, 0xABCD, 0xFFFC);
    reset(cpu);
    assert_eq(cpu->status, 0b00100100);
    assert_eq(cpu->reg_a, 0);
    assert_eq(cpu->reg_x, 0);
    assert_eq(cpu->reg_y, 0);
    assert_eq(cpu->program_counter, 0xABCD);
    free(cpu);
}

void test_load(void)
{
    CPU *cpu = new_cpu();
    uint8_t program[3] = { 0xAB, 0xCD, 0xDE };
    load(cpu, program, 3);

    uint8_t program_start_lsb = PROGRAM_START & 0xFF;
    uint8_t program_start_msb = PROGRAM_START >> 8;
    
    assert_eq(cpu->memory[0xFFFC], program_start_lsb);
    assert_eq(cpu->memory[0xFFFD], program_start_msb);
    assert_eq(cpu->memory[PROGRAM_START], 0xAB);
    assert_eq(cpu->memory[PROGRAM_START + 1], 0xCD);
    assert_eq(cpu->memory[PROGRAM_START + 2], 0xDE);
    free(cpu);
}

void test_run(void)
{
    CPU *cpu = new_cpu();
    populate_inst_list();
    
    uint8_t program[] = { 0xA9, 0xC0, 0xAA, 0xE8, 0x00 };
    int program_length = sizeof(program) / sizeof(program[0]);
    load(cpu, program, program_length);
    run_for_testing(cpu);

    assert_eq(cpu->reg_a, 0xC0);
    assert_eq(cpu->reg_x, 0xC1);
    free(cpu);
}

void test_set_unset_flag(void)
{
    CPU *cpu = new_cpu();
    set_flag(cpu, NEGATIVE_FLAG);

    assert_eq(cpu->status, 0b10000000);

    set_flag(cpu, CARRY_FLAG);
    assert_eq(cpu->status, 0b10000001);

    unset_flag(cpu, NEGATIVE_FLAG);
    assert_eq(cpu->status, 0b00000001);
    free(cpu);
}

void test_is_set(void)
{
    CPU *cpu = new_cpu();
    set_flag(cpu, NEGATIVE_FLAG);
    assert_eq(is_set(cpu, NEGATIVE_FLAG), true);
    assert_eq(is_set(cpu, ZERO_FLAG), false);
    free(cpu);
}

void test_get_stack_addr(void)
{
    CPU *cpu = new_cpu();
    cpu->stack_pointer = 0xAA;
    assert_eq(get_stack_addr(cpu), 0x01AA);
    free(cpu);
}

void test_stack(void)
{
    CPU *cpu = new_cpu();
    cpu->stack_pointer = 0xFF;
    stack_push_u16(cpu, 0xAABB);
    assert_eq(stack_pull_u16(cpu), 0xAABB);
    free(cpu);
}