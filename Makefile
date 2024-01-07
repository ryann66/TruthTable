CC = g++
CFLAGS = -g
OBJS = table.o tokens.o parser.o errorHandler.o bufferedErrorStream.o

all: table

table: table.o
	$(CC) $(CFLAGS) -o table $(OBJS)

table.o: table.cc tokens.o parser.o errorHandler.o
	$(CC) $(CFLAGS) -c $<

tokens.o: tokens.cc tokens.h errorHandler.o
	$(CC) $(CFLAGS) -c $<

parser.o: parser.cc parser.h tokens.o errorHandler.o
	$(CC) $(CFLAGS) -c $<
	
errorHandler.o: errorHandler.cc errorHandler.h bufferedErrorStream.o
	$(CC) $(CFLAGS) -c $<

bufferedErrorStream.o: bufferedErrorStream.c bufferedErrorStream.h
	gcc $(CFLAGS) -c $<

clean:
	/bin/rm -f *.a *.o table