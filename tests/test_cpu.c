#include "test_framework.h"
#include "../src/cpu.h"

int main(void);
void test_new(void);
void test_read_and_write_mem(void);
void test_reset(void);
void test_load(void);

int successful_tests = 0;
int failed_tests = 0;

int main(void)
{
    test_new();
    test_read_and_write_mem();
    test_reset();
    test_load();
    end_tests();
}

void test_new(void)
{
    CPU cpu = new_cpu();
    assert_eq(cpu.status, 0);
    assert_eq(cpu.reg_a, 0);
    assert_eq(cpu.reg_x, 0);
    assert_eq(cpu.reg_y, 0);
    assert_eq(cpu.program_counter, 0);
}

void test_read_and_write_mem(void)
{
    CPU cpu = new_cpu();
    write_mem(&cpu, 0xAB, 0x8000);
    assert_eq(cpu.memory[0x8000], 0xAB);
    assert_eq(read_mem(&cpu, 0x8000), 0xAB);
    
    // 16 bit tests
    write_mem_u16(&cpu, 0xABCD, 0x8000);
    assert_eq(cpu.memory[0x8000], 0xCD);
    assert_eq(cpu.memory[0x8000 + 1], 0xAB);
    assert_eq(read_mem_u16(&cpu, 0x8000), 0xABCD);
}

void test_reset(void)
{
    CPU cpu = new_cpu();
    cpu.reg_a = 10;
    cpu.reg_x = 25;
    cpu.reg_y = 50;
    cpu.status = 64;
    write_mem_u16(&cpu, 0xABCD, 0xFFFC);
    reset(&cpu);
    assert_eq(cpu.status, 0);
    assert_eq(cpu.reg_a, 0);
    assert_eq(cpu.reg_x, 0);
    assert_eq(cpu.reg_y, 0);
    assert_eq(cpu.program_counter, 0xABCD);
}

void test_load(void)
{
    CPU cpu = new_cpu();
    uint8_t program[3] = { 0xAB, 0xCD, 0xDE };
    load(&cpu, program, 3);
    assert_eq(cpu.memory[0xFFFC], 0x00);
    assert_eq(cpu.memory[0xFFFD], 0x80);
    assert_eq(cpu.memory[0x8000], 0xAB);
    assert_eq(cpu.memory[0x8001], 0xCD);
    assert_eq(cpu.memory[0x8002], 0xDE);
}