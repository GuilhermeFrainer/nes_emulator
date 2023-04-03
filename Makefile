CC = gcc
SRCDIR = src
OBJDIR = obj
TEST = tests
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

nes_emulator: $(OBJS)
	$(CC) -g -o nes_emulator $(OBJS)

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/cpu.h $(OBJDIR)
	$(CC) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h $(OBJDIR)
	$(CC) -c $< -o $@

# Create an obj directory if there isn't one
$(OBJDIR):
	mkdir $@

test: $(OBJDIR)/test.o $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o
	$(CC) -o test  $(OBJDIR)/test.o $(OBJDIR)/cpu.o $(OBJDIR)/instructions.o

$(OBJDIR)/test.o: $(TEST)/test.c
	$(CC) -c $< -o $@

.PHONY: clean

clean:
	rm $(OBJDIR)/*.o