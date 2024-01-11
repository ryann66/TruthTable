#pragma once

#include <cstddef>

#define LONG_BYTE_LEN (64)
typedef unsigned long long int longByte;

class BitSequence {
public:
    // constructs a new empty BitSequence
    BitSequence() : len(0), bits(nullptr) { }

    // constructs a BitSequence that starts with zeros but then flips
    // between zero and one every altLen bits
    // If altLen == 0, then all bits will be initialized to 0
    BitSequence(size_t len, size_t altLen);

    BitSequence(const BitSequence& other);

    BitSequence& operator= (const BitSequence& other);

    // indexes into the bit sequence to get the value of an individual bit
    // zero based indexing
    bool operator[] (size_t bit) const;

private:
    // constructs a new BitSequence with the given length of sequence but does
    // not offer any guarantee on the values of the bits
    BitSequence(size_t len);

    // number of bits in array
    size_t len;
    longByte* bits;

    friend BitSequence operator| (const BitSequence& lhs, const BitSequence& rhs);

    friend BitSequence operator& (const BitSequence& lhs, const BitSequence& rhs);

    friend BitSequence operator^ (const BitSequence& lhs, const BitSequence& rhs);

    friend BitSequence operator~ (const BitSequence& lb);
};