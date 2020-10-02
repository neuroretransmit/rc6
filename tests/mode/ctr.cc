#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rc6/cipher/rc6.h"
#include "rc6/mode/ctr.h"
#include "rc6/mode/ecb.h"

using namespace ::testing;

// TODO: Add DEBUG defines and spot check deeper at private variables/block transitions
TEST(Modes, CTREncryptDecryptZeroesForAll)
{
    RC6<BlockWordSize::BLOCK_128> cipher{};
    ECB<RC6<BlockWordSize::BLOCK_128>> ecb(cipher);
    // EW, we desperately need to clean this up
    CTR<ECB<RC6<BlockWordSize::BLOCK_128>>> ctr(ecb, block_byte_size<BlockWordSize::BLOCK_128>());
    const vector<u8> key(32);
    const vector<u8> EXPECTED(256);
    vector<u8> plain = EXPECTED;
    vector<u8> tag(16); // TODO: Fairly certain tag is just the counter in CTR mode. May have to classify this
                        // outside of a generic CTR or adapt implementation
    ctr.crypt(plain, key, tag);
    ASSERT_NE(plain, EXPECTED);
    ctr.crypt(plain, key, tag);
    ASSERT_EQ(plain, EXPECTED);
}

TEST(Modes, CTREncryptDecryptZeroesForAllParallel)
{
    RC6<BlockWordSize::BLOCK_128> cipher{};
    ECB<RC6<BlockWordSize::BLOCK_128>> ecb(cipher);
    // EW, we desperately need to clean this up
    CTR<ECB<RC6<BlockWordSize::BLOCK_128>>> ctr(ecb, block_byte_size<BlockWordSize::BLOCK_128>());
    const vector<u8> key(32);
    const vector<u8> EXPECTED(256);
    vector<u8> plain = EXPECTED;
    vector<u8> tag(16); // TODO: Fairly certain tag is just the counter in CTR mode. May have to classify this
                        // outside of a generic CTR or adapt implementation
    ctr.crypt_parallel(plain, key, tag);
    ASSERT_NE(plain, EXPECTED);
    ctr.crypt_parallel(plain, key, tag);
    ASSERT_EQ(plain, EXPECTED);
}

TEST(Modes, CTREncryptBlockExternallyForStream)
{
    RC6<BlockWordSize::BLOCK_128> cipher{};
    ECB<RC6<BlockWordSize::BLOCK_128>> ecb(cipher);
    // EW, we desperately need to clean this up
    CTR<ECB<RC6<BlockWordSize::BLOCK_128>>> ctr(ecb, block_byte_size<BlockWordSize::BLOCK_128>());

    const vector<u8> key(16);
    const vector<u8> EXPECTED(16);
    vector<u8> plain = EXPECTED;

    vector<u8> tag(16);
    vector<u8> counter(16);
    ctr.init_counter(counter, tag);

    // Encrypt
    ctr.crypt_block(plain, key, counter);
    ASSERT_NE(plain, EXPECTED);

    // Decrypt
    ctr.init_counter(counter, tag);
    ctr.crypt_block(plain, key, counter);
    ASSERT_EQ(plain, EXPECTED);
}
