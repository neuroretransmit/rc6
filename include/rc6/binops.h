#pragma once

#include <limits>
#include <climits>

#include "types.h"

using namespace std;

/**
 * Rotate a N-bit value left
 * @param word: value to rotate
 * @param shift: bits to roll
 */
template<class T> inline T rol(T word, int shift)
{
	return (word << shift) | (word >> (numeric_limits<T>::digits - shift));
}

/**
 * Rotate a N-bit value right
 * @param word: value to rotate
 * @param shift: bits to roll
 */
template<class T> inline T ror(T word, int shift)
{
	return (word >> shift) | (word << (numeric_limits<T>::digits - shift));
}

/**
 * Check if architecture is big or little endian
 */
inline bool is_big_endian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}

/**
 * Reverse endianness of a type
 * @param u: value to flip endianness of
 */
template <typename T> T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union{
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}
