CC = gcc
CFLAGS = -g -Wall

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


# Main file instructions
$(BIN): $(OBJS) $(BINDIR)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) -lmingw32 -lSDL2main -lSDL2

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(LIBDIR)/cpu.h $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(LIBDIR)/%.h $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

release: $(OBJS) $(BINDIR)
	$(CC) -O2 -o $(BIN) $(OBJS) -lmingw32 -lSDL2main -lSDL2

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
TESTFLAGS = -lmingw32 -lSDL2main -lSDL2

test: $(BINDIR)/test_cpu $(BINDIR)/test_instructions

test_log: $(BINDIR)/test_log

$(BINDIR)/test_cpu: $(TEST_REQS) $(TESTDIR)/test_cpu.c
	$(CC) $(TESTDIR)/test_cpu.c -o $(BINDIR)/test_cpu $(CPUOBJS) $(TESTFLAGS)

$(BINDIR)/test_instructions: $(TEST_REQS) $(TESTDIR)/test_instructions.c
	$(CC) $(TESTDIR)/test_instructions.c -o $(BINDIR)/test_instructions $(CPUOBJS) $(TESTFLAGS)

$(BINDIR)/test_log: $(TEST_REQS) $(TESTDIR)/test_log.c
	$(CC) $(TESTDIR)/test_log.c -o $(BINDIR)/test_log $(CPUOBJS) $(TESTFLAGS)


$(TESTDIR):
	mkdir $@


# Cleaning command
.PHONY: clean

clean:
	rm $(OBJDIR)/*.o $(BINDIR)/*.exe