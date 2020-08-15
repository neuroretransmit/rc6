#pragma once

#include <limits>

using std::numeric_limits;

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
