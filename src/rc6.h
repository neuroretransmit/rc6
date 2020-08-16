#pragma once

#include <cmath>
#include <cstdio>
#include <vector>
#include <limits>
#include <iostream>
#include <type_traits>

#include "types.h"
#include "binops.h"

using std::cerr;
using std::max;
using std::ceil;
using std::pow;
using std::vector;
using std::numeric_limits;


template<class T> class RC6 {
public:
    static const unsigned MAX_KEY_LEN = 2040;
    
    /// Binary expansion of e - 2
    static inline const T P = T(ceil((M_E - 2) * pow(2, numeric_limits<T>::digits)));
    /// Binary expansion of ϕ - 1 where ϕ is the golden ratio
    static inline const T Q = T(((1.618033988749895 - 1) * pow(2, numeric_limits<T>::digits)));
    
    /**
     * Encrypt @plaintext using @key for @r rounds
     * @param plaintext: plain text to encrypt
     * @param key: key bytes
     * @param half_rounds: number of half-rounds
     */
    static void encrypt(vector<T>& plaintext, vector<u8>& key, T half_rounds = 20)
    {
        vector<T> schedule(2 * half_rounds + 4);
        key_schedule(key, schedule, half_rounds);
        const T word_size_bits = numeric_limits<T>::digits;

        for (auto block = plaintext.begin(); block != plaintext.end(); block += 4) {
            block[B] += schedule[0];
            block[D] += schedule[1];

            for (T i = 1; i <= half_rounds; i++) {
                T t = rol(block[B] * (2 * block[B] + 1), log2(word_size_bits));
                T u = rol(block[D] * (2 * block[D] + 1), log2(word_size_bits));
                block[A] = rol((block[A] ^ t), u) + schedule[2 * i];
                block[C] = rol((block[C] ^ u), t) + schedule[2 * i + 1];
                T a = block[A];
                block[A] = block[B];
                block[B] = block[C];
                block[C] = block[D];
                block[D] = a;
            }
            
            block[A] += schedule[2 * half_rounds + 2];
            block[C] += schedule[2 * half_rounds + 3];
        }
    }

    /**
     * Decrypt @ciphertext using @key for @r rounds
     * @param plaintext: plain text to encrypt
     * @param key: key bytes
     * @param half_rounds: number of half-rounds
     */
    static void decrypt(vector<T>& ciphertext, vector<u8>& key, T half_rounds = 20)
    {
        vector<T> schedule(2 * half_rounds + 4);
        key_schedule(key, schedule, half_rounds);
        const T word_size_bits = numeric_limits<T>::digits;
        for (auto block = ciphertext.begin(); block != ciphertext.end(); block += 4) {
            block[C] -= schedule[2 * half_rounds + 3];
            block[A] -= schedule[2 * half_rounds + 2];

            for (T i = half_rounds; i >= 1; i--) {
                T d = block[D];
                block[D] = block[C];
                block[C] = block[B];
                block[B] = block[A];
                block[A] = d;
                T u = rol(block[D] * (2 * block[D] + 1), log2(word_size_bits));
                T t = rol(block[B] * (2 * block[B] + 1), log2(word_size_bits));
                block[C] = ror(block[C] - schedule[2 * i + 1], t) ^ u;
                block[A] = ror(block[A] - schedule[2 * i], u) ^ t;
            }

            block[D] -= schedule[1];
            block[B] -= schedule[0];
        }
    }
    
private:
    // Used for indices on blocks to match paper
    enum { A, B, C, D };

    /**
     * Create key schedule @S from input @key
     * @param key: key bytes
     * @param S: destination for key schedule
     * @param half_rounds: number of half-rounds
     */
    static void key_schedule(vector<u8>& key, vector<T>& schedule, T half_rounds = 20) {
        T key_len_bytes = key.size() / sizeof(u8);
        const T key_len_bits = key_len_bytes * 8;

        if (key_len_bits > MAX_KEY_LEN) {
            cerr << "ERROR: Key can't be greater than 2040 bits, got " << key_len_bits << ".\n";
            exit(1);
        }

        for ( ; key.size() % sizeof(T) != 0; key_len_bytes++)
            key.push_back(0x00);

        const T key_as_words_len = key.size() / sizeof(T);
        vector<T> little_endian_word_key(key.data(), static_cast<u8*>(key.data()) + key_len_bytes);
        const T v = 3 * max((unsigned) key_as_words_len, 2 * (unsigned) half_rounds + 4);
        T i = 0, j = 0;

        schedule[0] = P;
        for (i = 1; i < 2 * half_rounds + 3; i++)
            schedule[i] = schedule[i - 1] + Q;

        i = 0;
        T a = 0, b = 0;
        for (T s = 1; s <= v; s++) {
            a = schedule[i] = rol(schedule[i] + a + b, 3);
            b = little_endian_word_key[j] = rol(little_endian_word_key[j] + a + b, a + b);
            i = (i + 1) % (2 * half_rounds + 4);
            j = (j + 1) % key_as_words_len;
        }
    }
};

