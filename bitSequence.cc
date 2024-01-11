#include <cstddef>

#include "bitSequence.h"

// given a specific bit index, these macros give you the index of the longByte
// and which bit index inside of longByte to access
#define INDEX_OF_LONG_BYTE(b) ((b) / (LONG_BYTE_LEN))
#define INDEX_OF_BIT(b) ((b) % (LONG_BYTE_LEN))

BitSequence::BitSequence(size_t len, size_t altLen) : len(len) {
    
}

BitSequence::BitSequence(size_t len) : len(len) {

}

BitSequence::BitSequence(const BitSequence& other) {

}

BitSequence& BitSequence::operator= (const BitSequence& other) {

}

bool BitSequence::operator[] (size_t bit) const {

}

BitSequence operator| (const BitSequence& lhs, const BitSequence& rhs) {

}

BitSequence operator& (const BitSequence& lhs, const BitSequence& rhs) {

}

BitSequence operator^ (const BitSequence& lhs, const BitSequence& rhs) {

}

BitSequence operator~ (const BitSequence& lb) {

}