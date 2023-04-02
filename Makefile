VPATH = src

nes_emulator : main.o
	cc -g -o nes_emulator main.o

main.o : cpu.h vector.o

vector.o : vector.h