CC = g++
CFLAGS = -g
OBJS = table.o tokens.o
HEADERS = tokens.h

all: $(OBJS)
	$(CC) $(CFLAGS) -o table $(OBJS)

%.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	/bin/rm *.o table