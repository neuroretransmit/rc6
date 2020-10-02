#pragma once

#include <cstddef>
#include <queue>
#include <thread>
#include <vector>

#include "../types.h"

using std::min;
using std::queue;
using std::thread;
using std::vector;

/// CTR mode of operation
template<class V> class CTR
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
        vector<u8> counter;
        init_counter(counter, tag);

        // Encrypt blocks in CTR mode using the tag as the counter
        for (size_t i = 0; i < input.size(); i += BLOCK_SIZE)
            crypt_block(input, encryption_key, counter, i);
    }

    /**
     * Crypt used for both encrypt and decrypt in counter mode, parallelized
     * @param input: message to encrypt
     * @param encryption_key: message encryption key
     * @param tag: calculated tag to use as counter
     */
    void crypt_parallel(vector<u8>& input, const vector<u8>& encryption_key, const vector<u8>& tag)
    {
        const auto processor_count = std::thread::hardware_concurrency();
        queue<thread> threads;

        vector<u8> last_counter(16);
        init_counter(last_counter, tag);

        // for block in input
        // adjust counter forward to block number, spawn block on thread
        // constrained to max CPUs
        for (size_t i = 0; i < input.size(); i += BLOCK_SIZE) {
            if (threads.size() >= processor_count) {
                threads.front().join();
                threads.pop();
            }

            threads.push(thread([this, &input, encryption_key, last_counter, i] {
                vector<u8> counter = vector<u8>(last_counter.begin(), last_counter.end());
                crypt_block_parallel(input, encryption_key, counter, this->BLOCK_SIZE, this->cipher, i);
            }));

            for (size_t k = 0; k < 4; k++)
                if (++last_counter[k])
                    break;
        }

        while (threads.size()) {
            threads.front().join();
            threads.pop();
        }
    }

    /**
     * Crypt used for both encrypt/decrypt with externally tracked counter for use in streams
     * init_counter MUST be called on counter with tag before operating this
     * @param input_block: block to encrypt
     * @param encryption_key: message encryption key
     * @param counter: vector of u8 to store updating counter in
     * @param offset: only used internally for crypt() to move along the full input buffer
     */
    static void crypt_block_parallel(vector<u8>& input, const vector<u8>& encryption_key,
                                     const vector<u8>& counter, size_t block_size, V& cipher,
                                     size_t offset = 0)
    {
        vector<u8> block_key(block_size);
        block_key.insert(block_key.begin(), counter.begin(), counter.end());
        cipher.encrypt(block_key, encryption_key);

        for (size_t j = 0; j < min(block_size, input.size()); j++)
            input[offset + j] ^= block_key[j];
    }

    /**
     * Crypt used for both encrypt/decrypt with externally tracked counter for use in streams
     * init_counter MUST be called on counter with tag before operating this
     * @param input_block: block to encrypt
     * @param encryption_key: message encryption key
     * @param counter: vector of u8 to store updating counter in
     * @param offset: only used internally for crypt() to move along the full input buffer
     */
    void crypt_block(vector<u8>& input, const vector<u8>& encryption_key, vector<u8>& counter,
                     size_t offset = 0)
    {
        vector<u8> block_key(BLOCK_SIZE);
        block_key.insert(block_key.begin(), counter.begin(), counter.end());
        cipher.encrypt(block_key, encryption_key);

        for (size_t j = 0; j < min(BLOCK_SIZE, input.size()); j++)
            input[offset + j] ^= block_key[j];

        // TODO: This calculation should be done externally, so parallel blocks
        // can encrypt
        // Increment counter bytes
        for (size_t k = 0; k < 4; k++)
            if (++counter[k])
                break;
    }

    /**
     * Initialize counter to initial state before iterating over blocks
     * @param counter: vector of u8 to store tagged counter
     * @param tag: calculated tag for initial state of counter
     */
    static void init_counter(vector<u8>& counter, const vector<u8>& tag)
    {
        // Copy tag into counter
        counter = tag;
        // OR last byte of counter with 0x80
        counter[counter.size() - 1] |= 0x80;
    }

  private:
    V& cipher;
    size_t BLOCK_SIZE;
};
