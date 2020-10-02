#pragma once

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <vector>

#include "rc6/binops.h"

using std::ceil;
using std::cerr;
using std::max;
using std::pow;
using std::size_t;
using std::vector;

/// Rivest cipher 6 implementation
template<class T> class RC6 : CipherInterface<T>
{
  public:
    /// 2040-bit max key length
    const unsigned MAX_KEY_BIT_LEN = 8 * 255;

    /**
     * Constructor for RC6 block cipher.
     * @param r: number of half-rounds
     */
    RC6(T half_rounds = 20)
        : HALF_ROUNDS(half_rounds), WORD_BIT_LEN(numeric_limits<T>::digits),
          DEFAULT_ITERATION_LIMIT(2 * HALF_ROUNDS + 4)
    {
    }

    /**
     * Encrypt plaintext block using user-supplied key
     * @param block: plain text block to encrypt
     * @param key: key bytes
     */
    void encrypt(vector<u8>& block, const vector<u8>& key)
    {
        // Set up word-sized 'registers'
        T* block_words = (T*) block.data();
        T& a = block_words[0];
        T& b = block_words[1];
        T& c = block_words[2];
        T& d = block_words[3];

        // Create schedule
        // schedule called S in paper
        vector<T> schedule(DEFAULT_ITERATION_LIMIT);
        key_schedule(key, schedule);

        // Encrypt
        b += schedule[0];
        d += schedule[1];
        for (size_t i = 1; i <= HALF_ROUNDS; i++) {
            T t = rol(b * (2 * b + 1), log2(WORD_BIT_LEN));
            T u = rol(d * (2 * d + 1), log2(WORD_BIT_LEN));
            a = rol((a ^ t), u) + schedule[2 * i];
            c = rol((c ^ u), t) + schedule[2 * i + 1];
            T a_copy = a;
            a = b;
            b = c;
            c = d;
            d = a_copy;
        }
        a += schedule[DEFAULT_ITERATION_LIMIT - 2];
        c += schedule[DEFAULT_ITERATION_LIMIT - 1];
    }

    /**
     * Decrypt encrypted block using user-supplied key
     * @param block: encrypted block to decrypt
     * @param key: key bytes
     */
    void decrypt(vector<u8>& block, const vector<u8>& key)
    {
        // Set up word-sized 'registers'
        T* block_words = (T*) block.data();
        T& a = block_words[0];
        T& b = block_words[1];
        T& c = block_words[2];
        T& d = block_words[3];

        // Create key schedule
        vector<T> schedule(DEFAULT_ITERATION_LIMIT);
        key_schedule(key, schedule);

        // Decrypt
        c -= schedule[DEFAULT_ITERATION_LIMIT - 1];
        a -= schedule[DEFAULT_ITERATION_LIMIT - 2];
        for (size_t i = HALF_ROUNDS; i >= 1; i--) {
            T d_copy = d;
            d = c;
            c = b;
            b = a;
            a = d_copy;
            T u = rol(d * (2 * d + 1), log2(WORD_BIT_LEN));
            T t = rol(b * (2 * b + 1), log2(WORD_BIT_LEN));
            c = ror(c - schedule[2 * i + 1], t) ^ u;
            a = ror(a - schedule[2 * i], u) ^ t;
        }
        d -= schedule[1];
        b -= schedule[0];
    }

    virtual size_t block_size() { return sizeof(T); }

#ifndef DEBUG
  private:
#endif
    // word_bit_leng called w in paper
    /// Word bit-length
    const T WORD_BIT_LEN;
    /// Number of half-rounds
    const T HALF_ROUNDS;
    /// Default iteration limit for key scheduling
    const size_t DEFAULT_ITERATION_LIMIT;

    /// TODO: Make these variables optionally configurable, see how changing affects confusion/diffusion
    /// Binary expansion of e - 2
    const T P = T(ceil((M_E - 2) * pow(2, numeric_limits<T>::digits)));
    /// Binary expansion of the golden ratio - 1
    const T Q = T(((1.618033988749895 - 1) * pow(2, numeric_limits<T>::digits)));

    /**
     * Create key schedule S from user-supplied key
     * @param key: key bytes
     * @param S: destination for key schedule
     */
    void key_schedule(const vector<u8>& key, vector<T>& schedule)
    {
        // Copy key to not modify original
        vector<u8> key_copy = key;
        // key_bit_len called b in paper
        const size_t key_bit_len = key_copy.size() * 8;

        // TODO: Use exception for programs using this as a library
        if (key_bit_len > MAX_KEY_BIT_LEN) {
            cerr << "ERROR: Key can't be greater than 2040 bits, got " << key_bit_len << ".\n";
            exit(1);
        }

        // Pad to word length
        while (key_copy.size() % sizeof(T))
            key_copy.push_back(0);

        // total_words called c in paper
        const size_t total_words = key_copy.size() / sizeof(T);

        // le_word_key called L in paper
        T* le_word_key = (T*) key_copy.data();

        // Ensure bytes are loaded little endian
        if (is_big_endian())
            for (size_t i = 0; i < total_words; i++)
                le_word_key[i] = swap_endian(le_word_key[i]);

        // num_iterations called v in paper
        const size_t num_iterations = 3 * max((T) total_words, (T) DEFAULT_ITERATION_LIMIT);
        T schedule_index = 0, word_index = 0;

        // Create initial schedule
        schedule[0] = P;
        for (schedule_index = 1; schedule_index <= 2 * HALF_ROUNDS + 3; schedule_index++)
            schedule[schedule_index] = schedule[schedule_index - 1] + Q;

        // Create schedule for determined iterations
        schedule_index = 0;
        T A = 0, B = 0;
        // iteration called s in paper
        for (size_t iteration = 1; iteration <= num_iterations; iteration++) {
            A = schedule[schedule_index] = rol(schedule[schedule_index] + A + B, 3);
            B = le_word_key[word_index] = rol(le_word_key[word_index] + A + B, A + B);

            // Wrapped indices for schedule/little endian word key
            schedule_index = (schedule_index + 1) % DEFAULT_ITERATION_LIMIT;
            word_index = (word_index + 1) % total_words;
        }
    }
};
