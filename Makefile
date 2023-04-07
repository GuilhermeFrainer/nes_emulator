CC = gcc
CFLAGS = -g -Wall

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = tests

# File collections
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
BIN = $(BINDIR)/nes_emulator


# Main file instructions
$(BIN): $(OBJS) $(BINDIR)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/cpu.h $(OBJDIR)
	$(CC) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h $(OBJDIR)
	$(CC) -c $< -o $@

# Create obj and bin directories if they don't exist
$(OBJDIR):
	mkdir $@

$(BINDIR):
	mkdir $@

# Command to run program
run: $(BIN)
	./bin/nes_emulator


# TESTS
TEST_REQS = $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o $(TESTDIR)/test_framework.h $(BINDIR)
CPUOBJS = $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o

test: $(BINDIR)/test_cpu $(BINDIR)/test_instructions

$(BINDIR)/test_cpu: $(TEST_REQS) $(TESTDIR)/test_cpu.c
	$(CC) $(TESTDIR)/test_cpu.c -o $(BINDIR)/test_cpu $(CPUOBJS)

$(BINDIR)/test_instructions: $(TEST_REQS) $(TESTDIR)/test_instructions.c
	$(CC) $(TESTDIR)/test_instructions.c -o $(BINDIR)/test_instructions $(CPUOBJS)

$(TESTDIR):
	mkdir $@


# Cleaning command
.PHONY: clean

clean:
	rm $(OBJDIR)/*.o $(BINDIR)/*.exe