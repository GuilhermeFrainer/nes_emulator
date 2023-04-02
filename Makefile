VPATH = src

nes_emulator : main.o
	cc -g -o nes_emulator main.o

main.o : cpu.o vector.o

vector.o : vector.h

cpu.o : cpu.h instructions.o

instructions.o : cpu.h instructions.h