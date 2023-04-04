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
test: $(BINDIR)/test_cpu

$(BINDIR)/test_cpu: $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o $(TESTDIR)/test_framework.h $(TESTDIR)/test_cpu.c $(BINDIR) 
	$(CC) $(TESTDIR)/test_cpu.c -o $(BINDIR)/test_cpu $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o

$(TESTDIR):
	mkdir $@


# Cleaning command
.PHONY: clean

clean:
	rm $(OBJDIR)/*.o $(BINDIR)/*.exe