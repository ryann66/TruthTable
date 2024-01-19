CC = g++
CFLAGS = -Os
OBJS = table.o tokens.o parser.o errorHandler.o bufferedErrorStream.o interpreter.o bitSequence.o tokenMatcher.o helper.o tablePrinter.o
DFLAGS = -g -O0

debug: override CFLAGS+=$(DFLAGS)
debug: all

build: clean
build: all

all: table

# link
table: $(OBJS)
	$(CC) $(CFLAGS) -o table $(OBJS)

table.o: table.cc tokens.h parser.h interpreter.h
	$(CC) $(CFLAGS) -c $<

tokens.o: tokens.cc tokens.h errorHandler.h tokenMatcher.h
	$(CC) $(CFLAGS) -c $<

parser.o: parser.cc parser.h tokens.h errorHandler.h
	$(CC) $(CFLAGS) -c $<

interpreter.o: interpreter.cc interpreter.h tokens.h bitSequence.h errorHandler.h
	$(CC) $(CFLAGS) -c $<
	
errorHandler.o: errorHandler.cc errorHandler.h tokens.h bufferedErrorStream.h terminalSequences.h
	$(CC) $(CFLAGS) -c $<

bufferedErrorStream.o: bufferedErrorStream.c bufferedErrorStream.h
	gcc $(CFLAGS) -c $<

bitSequence.o: bitSequence.cc bitSequence.h
	$(CC) $(CFLAGS) -c $<

tokenMatcher.o: tokenMatcher.cc tokenMatcher.h
	$(CC) $(CFLAGS) -c $<

helper.o: helper.cc helper.h
	$(CC) $(CFLAGS) -c $<

tablePrinter.o: tablePrinter.cc tablePrinter.h bitSequence.h terminalSequences.h
	$(CC) $(CFLAGS) -c $<

errorHandler.h: tokens.h
interpreter.h: tokens.h
parser.h: tokens.h
tokenMatcher.h: tokens.h

clean:
	/bin/rm -f *.a *.o table