#include <stdio.h>
#include <stdint.h>
#include "cpu.h"

int main(void)
{
    uint8_t program[] = {0xa9, 0xc0, 0xaa, 0xe8, 0x00};
    CPU cpu = new_cpu();
    interpret(&cpu, program);
    printf("A reg: %i\nX reg: %i\n", cpu.reg_a, cpu.reg_x);
}