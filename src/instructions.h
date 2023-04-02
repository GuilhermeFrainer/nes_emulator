#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

void brk(CPU *cpu);
void inx(CPU *cpu);
void lda(CPU *cpu, uint8_t value);
void tax(CPU *cpu);
void update_zero_and_negative_flags(CPU *cpu, uint8_t result);

#endif