#include <stdint.h>

typedef struct CPU {
    int8_t register_a;
    int8_t status;
    int16_t program_counter;
} CPU;

CPU get_cpu(void) {
    CPU cpu;
    return cpu;
}

void cpu_interpret(CPU *cpu, int8_t test_value) {
    cpu->register_a = test_value;
    return;
}