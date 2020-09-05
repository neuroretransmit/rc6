#pragma once

#include <cstddef>
#include <vector>

#include "../types.h"

using std::min;
using std::vector;

/// CTR mode of operation
// TODO: See if specification different than default CTR, otherwise move into
// modes and genericize
template <class V> class CTR
{
  public:
    /**
     * Constructor for CTR
     * @param cipher: cipher to use
     * @param block_size: block cipher size
     */
    CTR(V& cipher, size_t block_size) : cipher(cipher), BLOCK_SIZE(block_size) {}

    /**
     * Crypt used for both encrypt and decrypt in counter mode
     * @param input: message to encrypt
     * @param encryption_key: message encryption key
     * @param tag: calculated tag to use as counter
     */
    void crypt(vector<u8>& input, const vector<u8>& encryption_key, const vector<u8>& tag)
    {
        vector<u8> counter = tag;
        counter[counter.size() - 1] |= 0x80;

        for (size_t i = 0; i < input.size(); i += BLOCK_SIZE) {
            vector<u8> key(BLOCK_SIZE);
            key.insert(key.begin(), counter.begin(), counter.end());
            cipher.encrypt(key, encryption_key);

            for (size_t j = 0; j < min(BLOCK_SIZE, input.size() - i); j++)
                input[i + j] = input[i + j] ^ key[j];

            for (size_t k = 0; k < 4; k++)
                if (++counter[k])
                    break;
        }
    }

  private:
    V& cipher;
    size_t BLOCK_SIZE;
};
