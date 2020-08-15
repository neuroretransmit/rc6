#include <gtest/gtest.h>

#include "rc6.h"

using rc6::RC6;

TEST(RC6, MagicConstantP32)
{
	ASSERT_EQ(0xB7E15163, rc6::P<u32>);
}

TEST(RC6, MagicConstantQ32)
{
	ASSERT_EQ(0x9E3779B9, rc6::Q<u32>);
}

TEST(RC6, KeyAbove2048Bits)
{
	vector<u8> key((RC6::MAX_KEY_LEN + 8) / 8);
	vector<u32> plain(16);
	ASSERT_EXIT(RC6::encrypt(plain, key), ::testing::ExitedWithCode(1), "Key can't be greater than 2040 bits");
}

/* TODO: Fix 8-bit and 16-bit words
   TEST(RC6, WordSize8Bit)
   {
   vector<u8> key(256 / 8);
   vector<u8> plain(16);
   vector<u8> encrypted = plain;
   RC6::encrypt(encrypted, key);
   ASSERT_NE(plain, encrypted);
   vector<u8> decrypted = encrypted;
   RC6::decrypt(decrypted, key);
   ASSERT_EQ(plain, decrypted);
   }

   TEST(RC6, WordSize16Bit)
   {
   vector<u8> key(256 / 8);
   vector<u16> plain(16);
   vector<u16> encrypted = plain;
   RC6::encrypt(encrypted, key);
   ASSERT_NE(plain, encrypted);
   vector<u16> decrypted = encrypted;
   RC6::decrypt(decrypted, key);
   ASSERT_EQ(plain, decrypted);
   }
   */

TEST(RC6, WordSize32Bit)
{
	vector<u8> key(256 / 8);
	vector<u32> plain(16);
	vector<u32> encrypted = plain;
	RC6::encrypt(encrypted, key);
	ASSERT_NE(plain, encrypted);
	vector<u32> decrypted = encrypted;
	RC6::decrypt(decrypted, key);
	ASSERT_EQ(plain, decrypted);
}

TEST(RC6, WordSize64Bit)
{
	vector<u8> key(256 / 8);
	vector<u64> plain(16);
	vector<u64> encrypted = plain;
	RC6::encrypt(encrypted, key);
	ASSERT_NE(plain, encrypted);
	vector<u64> decrypted = encrypted;
	RC6::decrypt(decrypted, key);
	ASSERT_EQ(plain, decrypted);
}
