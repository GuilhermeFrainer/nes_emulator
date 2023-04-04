CC = gcc
CFLAGS = -g -Wall

# Directories
SRCDIR = src
OBJDIR = obj
TESTDIR = tests

# File collections
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))


# Main file instructions
nes_emulator: $(OBJS)
	$(CC) $(CFLAGS) -o nes_emulator $(OBJS)

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/cpu.h $(OBJDIR)
	$(CC) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h $(OBJDIR)
	$(CC) -c $< -o $@

# Create an obj directory if there isn't one
$(OBJDIR):
	mkdir $@


# Testing-related instructions
test: $(OBJDIR)/test.o $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o
	$(CC) -o test  $(OBJDIR)/test.o $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o

$(OBJDIR)/test.o: $(TESTDIR)/test.c
	$(CC) -c $< -o $@

$(TESTDIR):
	mkdir $@


# Cleaning command
.PHONY: clean

clean:
	rm $(OBJDIR)/*.o