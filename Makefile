CC = gcc
OBJ = cpu.o instructions.o
VPATH = src tests

nes_emulator: main.o $(OBJ)
	$(CC) -g -o nes_emulator main.o $(OBJ)

main.o: main.c cpu.h

cpu.o: cpu.h cpu.c instructions.h

instructions.o: instructions.h instructions.c cpu.h

test.o: test.c cpu.h instructions.h

.PHONY: clean

clean:
	rm *.o