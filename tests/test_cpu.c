#include "test_framework.h"
#include "../src/cpu.h"

int main(void);
void test_new(void);

int successful_tests = 0;
int failed_tests = 0;

int main(void)
{
    test_new();
    end_tests();
}

void test_new(void)
{
    CPU cpu = new_cpu();
    assert_eq_u8(cpu.status, 1);
}