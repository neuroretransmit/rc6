#pragma once

#include <iterator>
#include <vector>

#include "../types.h"

using std::iterator;
using std::vector;

/// Electronic Code Book mode of operation
template <class T> class ECB : CipherInterface<T>
{
  public:
    ECB(T &cipher) : BLOCK_SIZE(4 * cipher.block_size()), cipher(cipher) {}

    void encrypt(vector<u8> &data, const vector<u8> &key)
    {
        size_t offset = 0;
        for (auto block = data.begin(); block != data.end(); block += BLOCK_SIZE, offset += BLOCK_SIZE) {
            vector<u8> chunk(block, block + BLOCK_SIZE);
            cipher.encrypt(chunk, key);

            for (size_t i = 0; i < BLOCK_SIZE; i++)
                data[offset + i] = chunk[i];
        }
    }

    void decrypt(vector<u8> &data, const vector<u8> &key)
    {
        size_t offset = 0;
        for (auto block = data.begin(); block != data.end(); block += BLOCK_SIZE, offset += BLOCK_SIZE) {
            vector<u8> chunk(block, block + BLOCK_SIZE);
            cipher.decrypt(chunk, key);

            for (size_t i = 0; i < BLOCK_SIZE; i++)
                data[offset + i] = chunk[i];
        }
    }

  private:
    const size_t BLOCK_SIZE;
    T &cipher;
};
