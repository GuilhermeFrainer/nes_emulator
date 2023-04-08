#include "test_framework.h"
#include "../src/instructions.h"
#include "../src/cpu.h"
#include <stdio.h>

int successful_tests = 0;
int failed_tests = 0;

void test_update_zero_and_negative_flags(void);
void test_get_operand_addr(void);

int main(void)
{
    test_update_zero_and_negative_flags();
    test_get_operand_addr();
    end_tests();
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