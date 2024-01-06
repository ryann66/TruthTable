CC = g++
CFLAGS = -g
OBJS = table.o tokens.o parser.o
HEADERS = tokens.h parser.h

all: $(OBJS)
	$(CC) $(CFLAGS) -o table $(OBJS)

%.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	/bin/rm *.o table