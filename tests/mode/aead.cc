#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>

#include "rc6/mode/aead.h"

using std::cout;
using std::hex;
using std::setfill;
using std::setw;

TEST(AEAD, DeriveKeys128)
{
    vector<u8> nonce(16);
    vector<u8> key_generating_key(16);
    vector<u8> message_authentication_key;
    vector<u8> message_encryption_key;
    AEAD<BlockWordSize::BLOCK_128> aead = AEAD<BlockWordSize::BLOCK_128>(key_generating_key);
    aead.derive_keys(message_authentication_key, message_encryption_key, nonce);
    ASSERT_EQ(message_authentication_key.size(), 128 / 8);
    ASSERT_EQ(message_encryption_key.size(), 128 / 8);
}

TEST(AEAD, Seal128)
{

    vector<u8> plaintext(256);
    vector<u8> aad(128);
    vector<u8> key_generating_key(16);
    AEAD<BlockWordSize::BLOCK_128> aead = AEAD<BlockWordSize::BLOCK_128>(key_generating_key);
    vector<u8> ciphertext = plaintext;
    aead.seal(ciphertext, aad);
    ASSERT_NE(plaintext, ciphertext);
}

TEST(AEAD, Open128)
{
    vector<u8> plaintext(256);
    vector<u8> aad(256);
    vector<u8> key_generating_key(16);
    AEAD<BlockWordSize::BLOCK_128> aead = AEAD<BlockWordSize::BLOCK_128>(key_generating_key);
    vector<u8> ciphertext = plaintext;
    aead.seal(ciphertext, aad);
    aead.open(ciphertext, aad);
    ASSERT_EQ(plaintext, ciphertext);
}

TEST(AEAD, DecipheredEqualsPlaintext128)
{
    vector<u8> plaintext(2, 1);
    vector<u8> aad(2, 2);
    vector<u8> key_generating_key(16, 1);
    AEAD<BlockWordSize::BLOCK_128> aead = AEAD<BlockWordSize::BLOCK_128>(key_generating_key);
    vector<u8> ciphertext = plaintext;
    aead.seal(ciphertext, aad);
    aead.open(ciphertext, aad);
    ASSERT_EQ(plaintext, ciphertext);
}

TEST(AEAD, DecipheredEqualsPlaintext256)
{
    vector<u8> plaintext(2, 1);
    vector<u8> aad(2, 2);
    vector<u8> key_generating_key(32, 1);
    AEAD<BlockWordSize::BLOCK_128> aead = AEAD<BlockWordSize::BLOCK_128>(key_generating_key);
    vector<u8> ciphertext = plaintext;
    aead.seal(ciphertext, aad);
    aead.open(ciphertext, aad);
    ASSERT_EQ(plaintext, ciphertext);
}

TEST(AEAD, SynchronousBlocks)
{
    vector<u8> plaintext(2, 1);
    vector<u8> aad(2, 2);
    vector<u8> key_generating_key(32, 1);
    AEAD<BlockWordSize::BLOCK_128> aead = AEAD<BlockWordSize::BLOCK_128>(key_generating_key);
    vector<u8> ciphertext = plaintext;
    aead.seal(ciphertext, aad, false);
    aead.open(ciphertext, aad, false);
    ASSERT_EQ(plaintext, ciphertext);
}
