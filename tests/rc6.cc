#include <gtest/gtest.h>

#include "rc6.h"


TEST(RC6, MagicConstantP32)
{
    RC6<u32> rc6 = RC6<u32>();
	ASSERT_EQ(0xB7E15163, rc6.P);
}

TEST(RC6, MagicConstantQ32)
{
    RC6<u32> rc6 = RC6<u32>();
	ASSERT_EQ(0x9E3779B9, rc6.Q);
}

TEST(RC6, KeyAbove2048Bits)
{
    RC6<u32> rc6 = RC6<u32>();
	vector<u8> key((rc6.MAX_KEY_LEN + 8) / 8);
	vector<u32> plain(16);
	ASSERT_EXIT(rc6.encrypt(plain, key), ::testing::ExitedWithCode(1), "Key can't be greater than 2040 bits");
}


TEST(RC6, WordSize32Bit)
{
    RC6<u32> rc6 = RC6<u32>();
	vector<u8> key(256 / 8);
	vector<u32> plain(16);
	vector<u32> encrypted = plain;
	rc6.encrypt(encrypted, key);
	ASSERT_NE(plain, encrypted);
	vector<u32> decrypted = encrypted;
	rc6.decrypt(decrypted, key);
	ASSERT_EQ(plain, decrypted);
}

TEST(RC6, WordSize64Bit)
{
    RC6<u64> rc6 = RC6<u64>();
	vector<u8> key(256 / 8);
	vector<u64> plain(16);
	vector<u64> encrypted = plain;
	rc6.encrypt(encrypted, key);
	ASSERT_NE(plain, encrypted);
	vector<u64> decrypted = encrypted;
	rc6.decrypt(decrypted, key);
	ASSERT_EQ(plain, decrypted);
}
