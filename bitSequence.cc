#include <cstddef>
#include <memory>

#include "bitSequence.h"

// given a specific bit index, these macros give you the index of the longByte
// and which bit index inside of longByte to access
#define INDEX_OF_LONG_BYTE(b) ((b) / (LONG_BYTE_LEN))
#define INDEX_OF_BIT(b) ((b) % (LONG_BYTE_LEN))

using std::shared_ptr;

BitSequence::BitSequence(size_t len, size_t altLen) : len(len), bits(new longByte[arrlen()]) {
    longByte* tmp = bits.get();
    size_t mask = 0b1;
    for (size_t i = 0; i < len;) {
        for (size_t j = 0; j < altLen && i < len; j++) {
            if (!mask) {
                mask = 0b1;
                tmp++;
            }
            *tmp &= (~mask);
            mask <<= 1;
            i++;
        }
        for (size_t j = 0; j < altLen && i < len; j++) {
            if (!mask) {
                mask = 0b1;
                tmp++;
            }
            *tmp |= mask;
            mask <<= 1;
            i++;
        }
    }
}

BitSequence::BitSequence(size_t len) : len(len), bits(new longByte[arrlen()]) { }

BitSequence::BitSequence(const BitSequence& other) : len(other.len), bits(other.bits) { }

BitSequence& BitSequence::operator= (const BitSequence& other) {
    len = other.len;
    bits = other.bits;
    return *this;
}

bool BitSequence::operator[] (size_t bit) const {
    longByte major = bits.get()[INDEX_OF_LONG_BYTE(bit)];
    size_t minor = INDEX_OF_BIT(bit);
    size_t mask = 0b1;
    mask <<= minor;
    return major & mask;
}

size_t BitSequence::arrlen() const {
    return len / LONG_BYTE_LEN + ((len % LONG_BYTE_LEN) ? 1 : 0);
}

BitSequence operator| (const BitSequence& lhs, const BitSequence& rhs) {
    if (lhs.len != rhs.len) return BitSequence();
    BitSequence r(lhs.len);
    for (size_t i = 0; i < lhs.arrlen(); i++) {
        r.bits.get()[i] = lhs.bits.get()[i] | rhs.bits.get()[i];
    }
    return r;
}

BitSequence operator& (const BitSequence& lhs, const BitSequence& rhs) {
    if (lhs.len != rhs.len) return BitSequence();
    BitSequence r(lhs.len);
    for (size_t i = 0; i < lhs.arrlen(); i++) {
        r.bits.get()[i] = lhs.bits.get()[i] & rhs.bits.get()[i];
    }
    return r;
}

BitSequence operator^ (const BitSequence& lhs, const BitSequence& rhs) {
    if (lhs.len != rhs.len) return BitSequence();
    BitSequence r(lhs.len);
    for (size_t i = 0; i < lhs.arrlen(); i++) {
        r.bits.get()[i] = lhs.bits.get()[i] ^ rhs.bits.get()[i];
    }
    return r;
}

BitSequence operator~ (const BitSequence& lb) {
    BitSequence r(lb.len);
    for (size_t i = 0; i < lb.arrlen(); i++) {
        r.bits.get()[i] = ~(lb.bits.get()[i]);
    }
    return r;
}