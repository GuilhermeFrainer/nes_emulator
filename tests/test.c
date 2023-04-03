#include "minunit.h"
#include "../src/instructions.h"
#include "../src/cpu.h"
#include <stdio.h>

int tests_run = 0;

// cpu.c tests
char *test_new_cpu()
{
    CPU cpu = new_cpu();
    mu_assert("cpu.status not initialized to 0", cpu.status == 0);
    mu_assert("cpu.program_counter not initialized to 0", cpu.program_counter == 0);
    mu_assert("cpu.reg_a not initialized to 0", cpu.reg_a == 0);
    mu_assert("cpu.reg_x not initialized to 0", cpu.reg_x == 0);
    mu_assert("cpu.reg_y not initialized to 0", cpu.reg_y == 0);

    return 0;
}

char *all_tests()
{
    test_new_cpu();
    return 0;
}

int main(void)
{
    char *result = all_tests();
    if (result != 0)
    {
        printf("Test failed: %s\n", result);
    }
    else
    {
        printf("Success! All tests passed!\n");
    }
}