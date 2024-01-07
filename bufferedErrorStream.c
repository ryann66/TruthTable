#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 256
#define NUM_LEN 4

char buffer[BUFFER_LENGTH];
unsigned int buffer_size = 0;

void flush() {
    if (buffer_size) {
        fprintf(stderr, buffer);
        buffer_size = 0;
        buffer[0] = '\0';
    }
}

int printString(const char* message) {
    int rem = strlen(message);
    int ret = rem;
    for (; buffer_size < BUFFER_LENGTH && *message; buffer_size++) {
        buffer[buffer_size] = *message;
        message++;
        rem--;
    }
    if (buffer_size == BUFFER_LENGTH) flush();
    if (*message) {
        if (rem >= BUFFER_LENGTH) {
            fprintf(stderr, message);
        } else {
            while (*message) {
                buffer[buffer_size] = *message;
                message++;
                buffer_size++;
            }
        }
    }
    return ret;
}

int printNum(unsigned char num) {
    int r;
    if (buffer_size + NUM_LEN >= BUFFER_LENGTH) flush();
    snprintf(buffer + buffer_size, NUM_LEN, "%hhu%n", num, &r);
    return r;
}

int printRepeated(char ch, int num) {
    int tmp = num;
    while (num) {
        for (; buffer_size < BUFFER_LENGTH && num; num--) {
            buffer[buffer_size] = ch;
            buffer_size++;
        }
        buffer[buffer_size] = '\0';
        if (buffer_size == BUFFER_LENGTH) flush();
    }
    return tmp;
}