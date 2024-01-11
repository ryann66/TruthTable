#pragma once

#include <cstddef>

typedef unsigned long long int longByte;

class BitSequence {
public:
    // constructs a new empty BitSequence
    BitSequence() : len(0), bits(nullptr) { }

    // constructs a BitSequence that starts with zeros but then flips
    // between zero and one every altLen bits
    // If altLen == 0, then will not flip
    BitSequence(size_t len, size_t altLen);

    BitSequence(const BitSequence& other);

    BitSequence& operator= (const BitSequence& other);

    // indexes into the bit sequence to get the value of an individual bit
    // zero based indexing
    bool operator[] (size_t bit) const;

private:
    // number of bits in array
    size_t len;
    longByte* bits;

    friend BitSequence operator| (const BitSequence& lhs, const BitSequence& rhs);

    friend BitSequence operator& (const BitSequence& lhs, const BitSequence& rhs);

    friend BitSequence operator~ (const BitSequence& lb);
};