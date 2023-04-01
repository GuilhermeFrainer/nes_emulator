VPATH = src

nes_emulator : main.o
	cc -o nes_emulator main.o

main.o : src/cpu.h

clean :
	rm *.o