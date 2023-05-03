#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include "../lib/cpu.h"
#include <stdint.h>
#include <stdio.h>

#define assert_eq(actual, expected) \
    if (actual == expected) { successful_tests++; } \
    else { printf("%s failed at %s at line %i.\n", __FUNCTION__,  __FILE__, __LINE__); failed_tests++; }

#define end_tests() printf("Testing over:\nSucceeded: %i\nFailed: %i\n", successful_tests, failed_tests);

// Run program for testing purposes only i.e., with no io
void run_for_testing(CPU *cpu)
{
    while (1)
    {
        uint8_t opcode = mem_read(cpu, cpu->program_counter);
        interpret(cpu, opcode);
        if (opcode == 0x00)
        {
            return;
        }
    }
}

#endif