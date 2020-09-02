#pragma once

// AEAD = Authenticated Encryption with Additional Data

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>
#include <random>

#include "../types.h"
#include "polyval.h"
#include "../cipher/rc6.h"

using std::vector;
using std::cerr;
using std::independent_bits_engine;
using std::default_random_engine;
using std::generate;
using std::ref;
using std::numeric_limits;
using random_bytes_engine = independent_bits_engine<default_random_engine, CHAR_BIT, u8>;

template<class T> class AEAD
{
public:
    /**
     * Constructor for AEAD.
     * @param key_generating_key: key generating key to derive from
     */
    AEAD(const vector<u8>& key_generating_key) :
        KEY_GENERATING_KEY(key_generating_key)
    {
        if (KEY_GENERATING_KEY.size() != BLOCK_SIZE || (BLOCK_SIZE != 128 && BLOCK_SIZE != 256)) {
            cerr << "ERROR: Key size for AEAD must match block size. Expected " << BLOCK_SIZE << " bits, got " << KEY_GENERATING_KEY.size() << ".\n";
            exit(1);
        }
            
    }
    
    /**
     * Encrypt with provided nonce
     * @param nonce: user-provided nonce
     * @param plaintext: plaintext to encrypt
     * @param authenticated_data: authenticated_data
     */
    void encrypt(const vector<u8>& nonce, vector<u8>& plaintext, vector<u8>& authenticated_data)
    {
        size_t nonce_size = nonce.size();
        
        if (nonce_size != NONCE_SIZE) {
            cerr << "ERROR: Nonce must be 96-bits, got " << nonce_size << ".\n";
            exit(1);
        }
        
        vector<u8> message_authentication_key;
        vector<u8> message_encryption_key;
        derive_keys(message_authentication_key, message_encryption_key, nonce);
        u64 plaintext_size = plaintext.size();
        u64 authenticated_data_size = authenticated_data.size();
        
        if (plaintext_size > MAX_DATA_SIZE) {
            cerr << "ERROR: Plaintext must be < 64GB, got " << plaintext_size << ".\n";
            exit(1);
        } else if (authenticated_data_size > MAX_DATA_SIZE) {
            cerr << "ERROR: Authenticated data must be < 64GB, got " << authenticated_data_size << ".\n";
            exit(1);
        }
        
        vector<u8> length_block(16);
        u64* length_block_u64 = (u64*) length_block.data();
        length_block_u64[0] = authenticated_data_size * 8;
        length_block_u64[1] = plaintext_size * 8;
        
        while (plaintext.size() % 16)
            plaintext.push_back(0);
        
        while (authenticated_data.size() % 16)
            authenticated_data.push_back(0);
    }
    
    /**
     * Encrypt with generated nonce
     * @param plaintext: plaintext to encrypt
     * @param authenticated_data: authenticated_data
     */
    void encrypt(vector<u8>& plaintext, vector<u8>& authenticated_data)
    {
        // Generate random nonce -- completely disallow misuse
        vector<u8> nonce(NONCE_SIZE);
        random_bytes_engine rbe;
        generate(begin(nonce), end(nonce), ref(rbe));
        encrypt(nonce, plaintext, authenticated_data);
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
        
        ctr_block.insert(ctr_block.end(), nonce.begin(), nonce.end());
        rc6.encrypt(ctr_block, KEY_GENERATING_KEY);
    }
    
    /**
     * Derive message authentication/encryption keys
     * @param message_authentication_key: reference to storage location of message authentication key
     * @param message_encryption_key: reference to storage location of message authentication key
     * @param nonce: nonce
     */
    void derive_keys(vector<u8>& message_authentication_key, vector<u8>& message_encryption_key, const vector<u8>& nonce)
    {
        RC6<T> rc6 = RC6<T>();
        u32 key_ctr; 
        
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
        
        if (KEY_GENERATING_KEY.size() == 256) {
            for (key_ctr = 4; key_ctr < 6; key_ctr++) {
                vector<u8> ctr_block;
                encrypt_key_ctr_block(rc6, ctr_block, key_ctr, nonce);
                
                for (size_t j = 0; j < 8; j++)
                    message_encryption_key.push_back(ctr_block[j]);
            }
        }
    }
    
private:
    const size_t NONCE_SIZE = 96;
    const size_t BLOCK_SIZE = numeric_limits<T>::digits * 4;
    const size_t MESSAGE_ENCRYPTION_KEY_LEN = BLOCK_SIZE;
    const size_t MESSAGE_AUTHENTICATION_KEY_LEN = 128;
    const size_t MAX_DATA_SIZE = pow(2, 36);

    const vector<u8>& KEY_GENERATING_KEY; 
};
