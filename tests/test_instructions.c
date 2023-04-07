#include "test_framework.h"
#include "../src/instructions.h"
#include "../src/cpu.h"
#include <stdio.h>

int successful_tests = 0;
int failed_tests = 0;

void test_update_zero_and_negative_flags(void);

int main(void)
{
    test_update_zero_and_negative_flags();
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