#include <stdio.h>
#include "cpu.h"

int main(void)
{
    CPU cpu = get_cpu();
    cpu_interpret(&cpu, 5);

}