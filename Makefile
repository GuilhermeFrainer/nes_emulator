nes_emulator : main.o
				cc -o nes_emulator main.o

main.o : main.c cpu.h
		cc -c main.c

clean :
	rm *.o