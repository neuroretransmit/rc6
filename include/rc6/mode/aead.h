#pragma once

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include "../cipher/rc6.h"
#include "aead/polyval.h"
#include "ctr.h"
#include "ecb.h"

using std::cerr;
using std::default_random_engine;
using std::equal;
using std::fill;
using std::generate;
using std::independent_bits_engine;
using std::numeric_limits;
using std::ref;
using std::vector;
using random_bytes_engine = independent_bits_engine<default_random_engine, CHAR_BIT, u8>;

/// Authenticated Encryption with Additional Data
template<class T> class AEAD
{
  public:
    /**
     * Constructor for AEAD.
     * @param key_generating_key: key generating key to derive from
     */
    AEAD(const vector<u8>& key_generating_key) : KEY_GENERATING_KEY(key_generating_key)
    {
        if ((KEY_GENERATING_KEY.size() != (128 / 8) && KEY_GENERATING_KEY.size() != (256 / 8))) {
            cerr << "ERROR: Key generating key for AEAD must be 16 or 32 bytes. Got "
                 << KEY_GENERATING_KEY.size() << ".\n";
            exit(1);
        }
    }

    /**
     * Encrypt message with prepended nonce/appended tag
     * @param plaintext: plaintext to encrypt
     * @param aad: additional authenticated data
     */
    void seal(vector<u8>& plaintext, vector<u8>& aad)
    {
        // Generate random nonce -- completely disallow misuse
        vector<u8> nonce(NONCE_BYTE_LEN);
        random_bytes_engine rbe;
        generate(begin(nonce), end(nonce), ref(rbe));
        // Encrypt
        seal(plaintext, aad, nonce);
        // Insert nonce into beginning of  plaintext
        plaintext.insert(plaintext.begin(), nonce.begin(), nonce.end());
    }

    /**
     * Decrypt/authenticate message
     * @param ciphertext: ciphertext to decrypt
     * @param aad: additional authenticated data
     */
    void open(vector<u8>& ciphertext, vector<u8>& aad)
    {
        size_t ciphertext_size = ciphertext.size();

        // TODO: Test
        if (ciphertext.size() < NONCE_BYTE_LEN) {
            cerr << "ERROR: Ciphertext to be at least the 96 bytes (nonce size), got " << ciphertext_size
                 << "\n";
            exit(1);
        }

        // Retrieve nonce
        const vector<u8> nonce(ciphertext.begin(), ciphertext.begin() + NONCE_BYTE_LEN);
        // Remove nonce from ciphertext
        ciphertext = vector<u8>(ciphertext.begin() + NONCE_BYTE_LEN, ciphertext.end());
        open(ciphertext, aad, nonce);
    }

#ifndef DEBUG
  private:
#endif

    const size_t NONCE_BYTE_LEN = 96 / 8;
    const size_t BLOCK_SIZE = sizeof(T) * 4;
    const size_t MAX_DATA_SIZE = pow(2, 36);
    const vector<u8>& KEY_GENERATING_KEY;

    /**
     * Calculate tag for authentication
     * @param message_encryption_key: vector to store message encryption key in
     * @param message_authentication_key: vector to store message authentication key in
     * @param plaintext: plaintext message
     * @param aad: authenticated additional data
     * @param nonce: nonce
     */
    vector<u8> get_tag(const vector<u8>& message_encryption_key, const vector<u8>& message_authentication_key,
                       const vector<u8>& plaintext, const vector<u8>& aad, const vector<u8>& nonce)
    {
        vector<u8> aad_plaintext_lengths(BLOCK_SIZE);
        in_place_update(aad_plaintext_lengths, (u64) aad.size() * 8, 8);

        // Digest
        Polyval<T> authenticator = Polyval<T>(message_authentication_key);
        authenticator.update(aad);
        authenticator.update(plaintext);
        authenticator.update(aad_plaintext_lengths);
        vector<u8> digest = authenticator.digest();

        // XOR first 96 bytes of digest with nonce
        for (size_t i = 0; i < nonce.size(); i++)
            digest[i] ^= nonce[i];

        digest[digest.size() - 1] &= ~0x80;

        // Encrypt digest
        RC6<T> cipher = RC6<T>();
        ECB<RC6<T>> ecb = ECB<RC6<T>>(cipher);
        ecb.encrypt(digest, message_encryption_key);

        return digest;
    }

    /**
     * Encrypt key counter block
     * @param rc6: cipher instance TODO: use abstract cipher interface
     * @param ctr_block: counter block
     * @param key_ctr: key counter
     * @param nonce: nonce
     */
    void encrypt_key_ctr_block(RC6<T>& rc6, vector<u8>& ctr_block, u32 key_ctr, const vector<u8>& nonce)
    {
        u8* byte_arr = (u8*) &key_ctr;

        for (size_t i = 0; i < sizeof(T); i++)
            ctr_block.push_back(byte_arr[i]);

        // Append nonce
        ctr_block.insert(ctr_block.end(), nonce.begin(), nonce.end());
        rc6.encrypt(ctr_block, KEY_GENERATING_KEY);
    }

    /**
     * Derive message authentication/encryption keys
     * @param message_authentication_key: reference to storage location of message
     * authentication key
     * @param message_encryption_key: reference to storage location of message
     * authentication key
     * @param nonce: nonce
     */
    void derive_keys(vector<u8>& message_authentication_key, vector<u8>& message_encryption_key,
                     const vector<u8>& nonce)
    {
        RC6<T> rc6 = RC6<T>();
        u32 key_ctr;

        // 128-bit key key generating key
        for (key_ctr = 0; key_ctr < 4; key_ctr++) {
            vector<u8> ctr_block;
            encrypt_key_ctr_block(rc6, ctr_block, key_ctr, nonce);

            for (size_t j = 0; j < 8; j++) {
                if (key_ctr < 2)
                    message_authentication_key.push_back(ctr_block[j]);
                else if (key_ctr >= 2)
                    message_encryption_key.push_back(ctr_block[j]);
            }
        }

        // 256-bit key generating key
        if (KEY_GENERATING_KEY.size() == 32) {
            for (key_ctr = 4; key_ctr < 6; key_ctr++) {
                vector<u8> ctr_block;
                encrypt_key_ctr_block(rc6, ctr_block, key_ctr, nonce);

                for (size_t j = 0; j < 8; j++)
                    message_encryption_key.push_back(ctr_block[j]);
            }
        }
    }

    /**
     * Encrypt message and append tag
     * @param plaintext: plaintext to encrypt
     * @param aad: additional authenticated data
     * @param nonce: user-provided nonce
     */
    void seal(vector<u8>& plaintext, vector<u8>& aad, const vector<u8>& nonce)
    {
        size_t nonce_size = nonce.size();

        if (nonce_size != NONCE_BYTE_LEN) {
            cerr << "ERROR: Nonce must be 96-bits, got " << nonce_size << ".\n";
            exit(1);
        }

        // Derive keys
        vector<u8> authentication_key;
        vector<u8> encryption_key;
        derive_keys(authentication_key, encryption_key, nonce);

        // Validate data is < 64GB for plaintext and AAD
        u64 plaintext_size = plaintext.size();
        u64 aad_size = aad.size();
        if (plaintext_size > MAX_DATA_SIZE) {
            cerr << "ERROR: Plaintext must be < 64GB, got " << plaintext_size << ".\n";
            exit(1);
        } else if (aad_size > MAX_DATA_SIZE) {
            cerr << "ERROR: Authenticated data must be < 64GB, got " << aad_size << ".\n";
            exit(1);
        }

        // Pad plaintext/AAD
        size_t plaintext_pad_len;
        for (plaintext_pad_len = 0; needs_padding(plaintext, BLOCK_SIZE); plaintext_pad_len++)
            plaintext.push_back(0);
        while (needs_padding(aad, BLOCK_SIZE))
            aad.push_back(0);

        // Calculate tag
        vector<u8> tag = get_tag(encryption_key, authentication_key, plaintext, aad, nonce);

        // Encrypt
        RC6<T> cipher = RC6<T>();
        ECB<RC6<T>> ecb = ECB<RC6<T>>(cipher);
        CTR<ECB<RC6<T>>> ctr = CTR<ECB<RC6<T>>>(ecb, BLOCK_SIZE);
        ctr.crypt(plaintext, encryption_key, tag);

        // Snip padding length
        plaintext.erase(plaintext.end() - plaintext_pad_len, plaintext.end());
        // Append tag
        plaintext.insert(plaintext.end(), tag.begin(), tag.end());
    }

    /**
     * Decrypt and authenticate message
     * @param ciphertext: ciphertext to decrypt and authenticate
     * @param aad: additional authenticated data
     * @param nonce: nonce
     */
    void open(vector<u8>& ciphertext, vector<u8>& aad, const vector<u8>& nonce)
    {
        // Extract tag/ciphertext
        vector<u8> tag(ciphertext.end() - BLOCK_SIZE, ciphertext.end());
        ciphertext = vector<u8>(ciphertext.begin(), ciphertext.end() - BLOCK_SIZE);

        // Pad ciphertext
        size_t ciphertext_pad_len = 0;
        for (ciphertext_pad_len = 0; needs_padding(ciphertext, BLOCK_SIZE); ciphertext_pad_len++)
            ciphertext.push_back(0);

        while (needs_padding(aad, BLOCK_SIZE))
            aad.push_back(0);

        // Derive keys
        vector<u8> authentication_key;
        vector<u8> encryption_key;
        derive_keys(authentication_key, encryption_key, nonce);

        // Decrypt
        RC6<T> cipher = RC6<T>();
        ECB<RC6<T>> ecb = ECB<RC6<T>>(cipher);
        CTR<ECB<RC6<T>>> ctr = CTR<ECB<RC6<T>>>(ecb, BLOCK_SIZE);
        ctr.crypt(ciphertext, encryption_key, tag);

        // Replace padded portion of decrypted text with zeroes for tag calculation
        for (size_t i = ciphertext.size() - ciphertext_pad_len; i < ciphertext.size(); i++)
            ciphertext[i] = 0;

        vector<u8> actual = get_tag(encryption_key, authentication_key, ciphertext, aad, nonce);

        // Snip padding to retrieve encrypted data without modification
        ciphertext = vector<u8>(ciphertext.begin(), ciphertext.end() - ciphertext_pad_len);

        // Authenticate data
        if (!equal(actual.begin(), actual.end(), tag.begin())) {
            cerr << "ERROR: Authentication failed.\n";
            exit(1);
        }
    }

    /**
     * Check if byte array needs to be padded to block size
     * @param bytes: byte array to check
     * @param block_size: target block size
     */
    bool needs_padding(const vector<u8>& bytes, size_t block_size)
    {
        return ((bytes.size() < block_size) && block_size - bytes.size()) || (bytes.size() % block_size);
    }

    void in_place_update(vector<u8>& bytes, u32 n)
    {
        bytes[0] = (u8) n;
        bytes[1] = (u8)(n >> 8);
        bytes[2] = (u8)(n >> 16);
        bytes[3] = (u8)(n >> 24);
    }

    void in_place_update(vector<u8>& bytes, u64 n, u32 offset)
    {
        bytes[offset] = (u8) n;
        bytes[offset + 1] = (u8)(n >> 8);
        bytes[offset + 2] = (u8)(n >> 16);
        bytes[offset + 3] = (u8)(n >> 24);
        bytes[offset + 4] = (u8)(n >> 32);
        bytes[offset + 5] = (u8)(n >> 40);
        bytes[offset + 6] = (u8)(n >> 48);
        bytes[offset + 7] = (u8)(n >> 56);
    }
};
