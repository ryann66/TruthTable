#pragma once

// flushes the buffer, printing to stderr
void flush();

// prints the given string, returns the number of characters written
int printString(char const* message);

// prints the given number, returns the number of characters written 
int printNum(unsigned char num);

// prints the given character num times, returns the number of characters printed
int printRepeated(char ch, int num);