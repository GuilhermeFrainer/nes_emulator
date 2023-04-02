CC = gcc
#CFLAGS = -g -o
#OBJ = cpu.o instructions.o
#OBJDIR = obj
VPATH = src tests

nes_emulator: main.o cpu.o instructions.o
	cc -g -o nes_emulator main.o cpu.o instructions.o

main.o: main.c cpu.h

cpu.o: cpu.h cpu.c instructions.h

instructions.o: instructions.h instructions.c cpu.h

test.o: test.c cpu.h instructions.h

#$(OBJ):
#	mkdir $@

.PHONY: clean

clean:
	rm *.o