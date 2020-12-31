# These are variables. We define CC, CFLAGS ,CCLINK, and OBJS
CC=gcc
CFLAGS=-g -Wall -std=c99
CCLINK=$(CC) # Read variable CC using $(CC)
OBJS=ransom.o
EXEC=prog.exe
RM=rm -rf *.o *.exe

# The first rule is invoked by default.
$(EXEC): $(OBJS)
	$(CCLINK) $(OBJS) -o $(EXEC)

# Full rule syntax with custom Shell commands
ransom.o: ransom.c 
	$(CC) $(CFLAGS) -c ransom.c

# We have a "clean" rule to delete any automatically generated files
clean:
	$(RM)
