#pragma once

#include <cmath>
#include <cstdio>
#include <cstddef>
#include <vector>
#include <iostream>

#include "types.h"
#include "binops.h"

using std::size_t;
using std::cerr;
using std::max;
using std::ceil;
using std::pow;
using std::vector;

template<class T> class RC6 {
public:
    const unsigned MAX_KEY_LEN = 2040;
    /// Binary expansion of e - 2
    const T P;
    /// Binary expansion of the golden ratio - 1
    const T Q;
    /// Word bit-length
    const T w;
    /// Number of half-rounds
    const T r;
    
    /**
     * Constructor for RC6 block cipher.
     * @param r: number of half-rounds
     */
    RC6(T r = 20) :
        P(T(ceil((M_E - 2) * pow(2, numeric_limits<T>::digits)))),
        Q(T(((1.618033988749895 - 1) * pow(2, numeric_limits<T>::digits)))),
        w(numeric_limits<T>::digits),
        r(r)
    {}
    
    /**
     * Encrypt plaintext @block using @key for @r rounds
     * @param block: plain text block to encrypt
     * @param key: key bytes
     */
    void encrypt(vector<u8>& block, const vector<u8>& key)
    {
        T* block_words = (T*) block.data();
        T& A = block_words[0];
        T& B = block_words[1];
        T& C = block_words[2];
        T& D = block_words[3];
        vector<T> S(2 * r + 4);
        key_schedule(key, S);
        B += S[0];
        D += S[1];
        
        for (size_t i = 1; i <= r; i++) {
            T t = rol(B * (2 * B + 1), log2(w));
            T u = rol(D * (2 * D + 1), log2(w));
            A = rol((A ^ t), u) + S[2 * i];
            C = rol((C ^ u), t) + S[2 * i + 1];
            T a = A;
            A = B; B = C; C = D; D = a;
        }
        
        A += S[2 * r + 2];
        C += S[2 * r + 3];
    }

    /**
     * Decrypt encrypted @block using @key for @r rounds
     * @param block: plain text to encrypt
     * @param key: key bytes
     */
    void decrypt(vector<u8>& block, const vector<u8>& key)
    {
        T* block_words = (T*) block.data();
        T& A = block_words[0];
        T& B = block_words[1];
        T& C = block_words[2];
        T& D = block_words[3];
        vector<T> S(2 * r + 4);
        key_schedule(key, S);
        C -= S[2 * r + 3];
        A -= S[2 * r + 2];

        for (size_t i = r; i >= 1; i--) {
            T d = D;
            D = C; C = B; B = A; A = d;
            T u = rol(D * (2 * D + 1), log2(w));
            T t = rol(B * (2 * B + 1), log2(w));
            C = ror(C - S[2 * i + 1], t) ^ u;
            A = ror(A - S[2 * i], u) ^ t;
        }

        D -= S[1];
        B -= S[0];
    }
    
private:
    /**
     * Create key schedule @S from input @key
     * @param key: key bytes
     * @param S: destination for key schedule
     */
    void key_schedule(const vector<u8>& key, vector<T>& S) {
        // Copy key to not augment original by appending bytes and reinterpreting pointer as L
        vector<u8> key_copy = key;
        const size_t b_bits = key_copy.size() * 8;

        if (b_bits > MAX_KEY_LEN) {
            cerr << "ERROR: Key can't be greater than 2040 bits, got " << b_bits << ".\n";
            exit(1);
        }

        while (key_copy.size() % sizeof(T) != 0)
            key_copy.push_back(0x00);

        const size_t c = key_copy.size() / sizeof(T);
        T* L = (T*) key_copy.data();
        const size_t v = 3 * max((T) c, 2 * r + 4);
        T i = 0, j = 0;

        S[0] = P;
        for (i = 1; i <= 2 * r + 3; i++)
            S[i] = S[i - 1] + Q;

        i = 0;
        T A = 0, B = 0;
        for (size_t s = 1; s <= v; s++) {
            A = S[i] = rol(S[i] + A + B, 3);
            B = L[j] = rol(L[j] + A + B, A + B);
            i = (i + 1) % (2 * r + 4);
            j = (j + 1) % c;
        }
    }
};

