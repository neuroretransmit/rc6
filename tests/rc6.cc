#include <gtest/gtest.h>

#include "rc6.h"

TEST(RC6, MagicConstantP32)
{
	ASSERT_EQ(0xB7E15163, RC6<u32>::P);
}

TEST(RC6, MagicConstantQ32)
{
	ASSERT_EQ(0x9E3779B9, RC6<u32>::Q);
}

TEST(RC6, KeyAbove2048Bits)
{
	vector<u8> key((RC6<u32>::MAX_KEY_LEN + 8) / 8);
	vector<u32> plain(16);
	ASSERT_EXIT(RC6<u32>::encrypt(plain, key), ::testing::ExitedWithCode(1), "Key can't be greater than 2040 bits");
}


TEST(RC6, WordSize32Bit)
{
	vector<u8> key(256 / 8);
	vector<u32> plain(16);
	vector<u32> encrypted = plain;
	RC6<u32>::encrypt(encrypted, key);
	ASSERT_NE(plain, encrypted);
	vector<u32> decrypted = encrypted;
	RC6<u32>::decrypt(decrypted, key);
	ASSERT_EQ(plain, decrypted);
}

TEST(RC6, WordSize64Bit)
{
	vector<u8> key(256 / 8);
	vector<u64> plain(16);
	vector<u64> encrypted = plain;
	RC6<u64>::encrypt(encrypted, key);
	ASSERT_NE(plain, encrypted);
	vector<u64> decrypted = encrypted;
	RC6<u64>::decrypt(decrypted, key);
	ASSERT_EQ(plain, decrypted);
}
