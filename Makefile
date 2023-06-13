CC = gcc
CFLAGS = -g -Wall
LIBFLAGS = -lSDL2main -lSDL2
WINVAR =

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
LIBDIR = lib
TESTDIR = tests

# File collections
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
BIN = $(BINDIR)/nes_emulator

# Checks if the operating system is Windows
# If it is, append '-lmingw32' to flags
ifeq ($(OS), Windows_NT)
	WINVAR += -lmingw32
endif

# Main file instructions
$(BIN): $(OBJS) $(BINDIR)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(WINVAR) $(LIBFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(LIBDIR)/cpu.h $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(LIBDIR)/%.h $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

release: $(OBJS) $(BINDIR)
	$(CC) -O2 -o $(BIN) $(OBJS) $(LIBFLAGS)

# Create obj and bin directories if they don't exist
$(OBJDIR):
	mkdir $@

$(BINDIR):
	mkdir $@

# Command to run program
run: $(BIN)
	./bin/nes_emulator


# TESTS
TEST_REQS = $(CPUOBJS) $(TESTDIR)/test_framework.h $(BINDIR)
CPUOBJS = $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o $(OBJDIR)/bus.o $(OBJDIR)/io.o $(OBJDIR)/cartridge.o
TESTFLAGS = -lSDL2main -lSDL2 -g -Wall

test: $(BINDIR)/test_cpu $(BINDIR)/test_instructions

test_log: $(BINDIR)/test_log

$(BINDIR)/test_cpu: $(TEST_REQS) $(TESTDIR)/test_cpu.c
	$(CC) $(TESTDIR)/test_cpu.c -o $(BINDIR)/test_cpu $(CPUOBJS) $(WINVAR) $(TESTFLAGS)

$(BINDIR)/test_instructions: $(TEST_REQS) $(TESTDIR)/test_instructions.c
	$(CC) $(TESTDIR)/test_instructions.c -o $(BINDIR)/test_instructions $(CPUOBJS) $(WINVAR) $(TESTFLAGS)

$(BINDIR)/test_log: $(TEST_REQS) $(TESTDIR)/test_log.c
	$(CC) $(TESTDIR)/test_log.c -o $(BINDIR)/test_log $(CPUOBJS) $(WINVAR) $(TESTFLAGS)


$(TESTDIR):
	mkdir $@


# Cleaning command
.PHONY: clean

clean:
	rm $(OBJDIR)/*.o $(BINDIR)/*.exe
