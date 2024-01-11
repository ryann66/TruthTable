CC = g++
CFLAGS = -g
OBJS = table.o tokens.o parser.o errorHandler.o bufferedErrorStream.o interpreter.o bitSequence.o

all: table

# link
table: $(OBJS)
	$(CC) $(CFLAGS) -o table $(OBJS)

table.o: table.cc tokens.h parser.h errorHandler.h
	$(CC) $(CFLAGS) -c $<

tokens.o: tokens.cc tokens.h errorHandler.h
	$(CC) $(CFLAGS) -c $<

parser.o: parser.cc parser.h tokens.h errorHandler.h
	$(CC) $(CFLAGS) -c $<

interpreter.o: interpreter.cc interpreter.h tokens.h bitSequence.h
	$(CC) $(CFLAGS) -c $<
	
errorHandler.o: errorHandler.cc errorHandler.h tokens.h bufferedErrorStream.h
	$(CC) $(CFLAGS) -c $<

bufferedErrorStream.o: bufferedErrorStream.c bufferedErrorStream.h
	gcc $(CFLAGS) -c $<

bitSequence.o: bitSequence.cc bitSequence.h
	gcc $(CFLAGS) -c $<

clean:
	/bin/rm -f *.a *.o table