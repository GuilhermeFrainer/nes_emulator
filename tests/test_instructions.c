#include "test_framework.h"
#include "../src/instructions.h"
#include "../src/cpu.h"

int successful_tests = 0;
int failed_tests = 0;

void test_get_instruction_from_opcode(void);
void test_populate_inst_list(void);
void test_update_zero_and_negative_flags(void);
void test_get_operand_addr(void);
void test_instructions(void);
void test_set_unset_flag(void);
void test_is_set(void);

int main(void)
{
    test_get_instruction_from_opcode();
    test_populate_inst_list();
    test_update_zero_and_negative_flags();
    test_get_operand_addr();
    test_instructions();
    test_set_unset_flag();
    test_is_set();
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
    CPU cpu = new_cpu();
    update_zero_and_negative_flags(&cpu, 0);
    assert_eq(cpu.status, 0b00000010);
    update_zero_and_negative_flags(&cpu, 0b10110010);
    assert_eq(cpu.status, 0b10000000);
    update_zero_and_negative_flags(&cpu, 0b00101101);
    assert_eq(cpu.status, 0b00000000);
}

void test_get_operand_addr(void)
{
    CPU cpu = new_cpu();
    cpu.program_counter = 15;
    assert_eq(get_operand_addr(&cpu, Immediate), 15);
    
    // Zero page
    cpu.reg_x = 5;
    cpu.reg_y = 10;
    write_mem(&cpu, 0xAB, cpu.program_counter);
    assert_eq(get_operand_addr(&cpu, ZeroPage), 0xAB);
    assert_eq(get_operand_addr(&cpu, ZeroPageX), 0xAB + 5);
    assert_eq(get_operand_addr(&cpu, ZeroPageY), 0xAB + 10);

    // Absolute
    cpu.program_counter = 100;
    cpu.reg_x = 50;
    cpu.reg_y = 25;
    write_mem_u16(&cpu, 0xBABA, cpu.program_counter);
    assert_eq(get_operand_addr(&cpu, Absolute), 0xBABA);
    assert_eq(get_operand_addr(&cpu, AbsoluteX), 0xBABA + 50);
    assert_eq(get_operand_addr(&cpu, AbsoluteY), 0xBABA + 25);

    // Indirect
    cpu.program_counter = 200;
    cpu.reg_x = 100;
    cpu.reg_y = 20;

    write_mem_u16(&cpu, 0x0120, cpu.program_counter);
    write_mem_u16(&cpu, 0xBAFC, 0x0120);
    assert_eq(get_operand_addr(&cpu, Indirect), 0xBAFC);

    cpu.program_counter = 300;
    write_mem(&cpu, 0xBC, cpu.program_counter);
    write_mem_u16(&cpu, 0xABBA, 0xBC + cpu.reg_x);
    assert_eq(get_operand_addr(&cpu, IndirectX), 0xABBA);

    write_mem_u16(&cpu, 0xBAAB, 0xBC);
    assert_eq(get_operand_addr(&cpu, IndirectY), 0xBAAB + cpu.reg_y);
}

void test_instructions(void)
{
    CPU cpu = new_cpu();
    populate_inst_list();

    cpu.reg_a = 0x80;
    write_mem(&cpu, 0x80, 0);
    adc(&cpu, Immediate);
    assert_eq(cpu.status, CARRY_FLAG | OVERFLOW_FLAG);
}

void test_set_unset_flag(void)
{
    CPU cpu = new_cpu();
    set_flag(&cpu, NEGATIVE_FLAG);

    assert_eq(cpu.status, 0b10000000);

    set_flag(&cpu, CARRY_FLAG);
    assert_eq(cpu.status, 0b10000001);

    unset_flag(&cpu, NEGATIVE_FLAG);
    assert_eq(cpu.status, 0b00000001);
}

void test_is_set(void)
{
    CPU cpu = new_cpu();
    set_flag(&cpu, NEGATIVE_FLAG);
    assert_eq(is_set(&cpu, NEGATIVE_FLAG), true);
    assert_eq(is_set(&cpu, ZERO_FLAG), false);
}