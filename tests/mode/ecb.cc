#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rc6/cipher/rc6.h"
#include "rc6/mode/ecb.h"

using namespace ::testing;

// TODO: Add DEBUG defines to spot check deeper
TEST(Modes, ECBEncryptDecryptZeroesForAll)
{
    RC6<BlockWordSize::BLOCK_128> cipher = RC6<BlockWordSize::BLOCK_128>();
    ECB<RC6<BlockWordSize::BLOCK_128>> ecb(cipher);
    const vector<u8> key(32);
    const vector<u8> EXPECTED(256);
    vector<u8> plain = EXPECTED;
    ecb.encrypt(plain, key);
    ASSERT_NE(plain, EXPECTED);
    ecb.decrypt(plain, key);
    ASSERT_EQ(plain, EXPECTED);
}
