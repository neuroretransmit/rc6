#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rc6.h"

void encrypted_to_bytes(const vector<u32>& encrypted, vector<u8>& output_bytes)
{
    for (u32 word : encrypted) {
        u8 bytes[4] = {0};
        bytes[0] = (uint8_t) word;
        bytes[1] = (uint8_t)(word >>= 8);
        bytes[2] = (uint8_t)(word >>= 8);
        bytes[3] = (uint8_t)(word >>= 8);
        
        for (size_t i = 0; i < sizeof(bytes); i++)
            output_bytes.push_back(bytes[i]);
    }
}

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
	const vector<u8> key((rc6.MAX_KEY_LEN + 8) / 8);
	vector<u32> plain(16);
	ASSERT_EXIT(rc6.encrypt(plain, key), ::testing::ExitedWithCode(1), "Key can't be greater than 2040 bits");
}

TEST(RC6, WordSize32Bit)
{
    RC6<u32> rc6 = RC6<u32>();
	const vector<u8> key(32);
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
	const vector<u8> key(32);
	vector<u64> plain(16);
	vector<u64> encrypted = plain;
	rc6.encrypt(encrypted, key);
	ASSERT_NE(plain, encrypted);
	vector<u64> decrypted = encrypted;
	rc6.decrypt(decrypted, key);
	ASSERT_EQ(plain, decrypted);
}

TEST(RC6, PaperTestVector1)
{
    RC6<u32> rc6 = RC6<u32>();
    const vector<u8> key(16);
    vector<u32> plain(4);
    vector<u32> encrypted = plain;
	rc6.encrypt(encrypted, key);
    vector<u8> ACTUAL;
    encrypted_to_bytes(encrypted, ACTUAL);
    ASSERT_THAT(ACTUAL, ::testing::ElementsAre(
        0x8f, 0xc3, 0xa5, 0x36,
        0x56, 0xb1, 0xf7, 0x78,
        0xc1, 0x29, 0xdf, 0x4e,
        0x98, 0x48, 0xa4, 0x1e)
    );
}

TEST(RC6, PaperTestVector2)
{
    RC6<u32> rc6 = RC6<u32>();
    const vector<u8> key = { 
        0x01, 0x23, 0x45, 0x67, 
        0x89, 0xab, 0xcd, 0xef, 
        0x01, 0x12, 0x23, 0x34, 
        0x45, 0x56, 0x67, 0x78 
    };
    vector<u32> plain = { 0x35241302, 0x79685746, 0xbdac9b8a, 0xf1e0dfce };
    vector<u32> encrypted = plain;
	rc6.encrypt(encrypted, key);
    vector<u8> ACTUAL;
    encrypted_to_bytes(encrypted, ACTUAL);
    ASSERT_THAT(ACTUAL, ::testing::ElementsAre(
        0x52, 0x4e, 0x19, 0x2f, 
        0x47, 0x15, 0xc6, 0x23, 
        0x1f, 0x51, 0xf6, 0x36, 
        0x7e, 0xa4, 0x3f, 0x18)
    );
}

TEST(RC6, PaperTestVector3)
{
    RC6<u32> rc6 = RC6<u32>();
    const vector<u8> key(24);
    vector<u32> plain(4);
    vector<u32> encrypted = plain;
	rc6.encrypt(encrypted, key);
    vector<u8> ACTUAL;
    encrypted_to_bytes(encrypted, ACTUAL);
    ASSERT_THAT(ACTUAL, ::testing::ElementsAre(
        0x6c, 0xd6, 0x1b, 0xcb, 
        0x19, 0x0b, 0x30, 0x38, 
        0x4e, 0x8a, 0x3f, 0x16, 
        0x86, 0x90, 0xae, 0x82)
    );
}

TEST(RC6, PaperTestVector4)
{
    RC6<u32> rc6 = RC6<u32>();
    const vector<u8> key = {
        0x01, 0x23, 0x45, 0x67, 
        0x89, 0xab, 0xcd, 0xef, 
        0x01, 0x12, 0x23, 0x34, 
        0x45, 0x56, 0x67, 0x78, 
        0x89, 0x9a, 0xab, 0xbc,
        0xcd, 0xde, 0xef, 0xf0
    };
    vector<u32> plain = { 0x35241302, 0x79685746, 0xbdac9b8a, 0xf1e0dfce };
    vector<u32> encrypted = plain;
	rc6.encrypt(encrypted, key);
    vector<u8> ACTUAL;
    encrypted_to_bytes(encrypted, ACTUAL);
    ASSERT_THAT(ACTUAL, ::testing::ElementsAre(
        0x68, 0x83, 0x29, 0xd0,
        0x19, 0xe5, 0x05, 0x04,
        0x1e, 0x52, 0xe9, 0x2a,
        0xf9, 0x52, 0x91, 0xd4)
    );
}

TEST(RC6, PaperTestVector5)
{
    RC6<u32> rc6 = RC6<u32>();
    const vector<u8> key(32);
    vector<u32> plain(4);
    vector<u32> encrypted = plain;
	rc6.encrypt(encrypted, key);
    vector<u8> ACTUAL;
    encrypted_to_bytes(encrypted, ACTUAL);
    ASSERT_THAT(ACTUAL, ::testing::ElementsAre(
        0x8f, 0x5f, 0xbd, 0x05,
        0x10, 0xd1, 0x5f, 0xa8,
        0x93, 0xfa, 0x3f, 0xda, 
        0x6e, 0x85, 0x7e, 0xc2)
    );
}

TEST(RC6, PaperTestVector6)
{
    RC6<u32> rc6 = RC6<u32>();
    const vector<u8> key = {
        0x01, 0x23, 0x45, 0x67, 
        0x89, 0xab, 0xcd, 0xef, 
        0x01, 0x12, 0x23, 0x34, 
        0x45, 0x56, 0x67, 0x78, 
        0x89, 0x9a, 0xab, 0xbc,
        0xcd, 0xde, 0xef, 0xf0,
        0x10, 0x32, 0x54, 0x76,
        0x98, 0xba, 0xdc, 0xfe
    };
    vector<u32> plain = { 0x35241302, 0x79685746, 0xbdac9b8a, 0xf1e0dfce };
    vector<u32> encrypted = plain;
	rc6.encrypt(encrypted, key);
    vector<u8> ACTUAL;
    encrypted_to_bytes(encrypted, ACTUAL);
    ASSERT_THAT(ACTUAL, ::testing::ElementsAre(
        0xc8, 0x24, 0x18, 0x16,
        0xf0, 0xd7, 0xe4, 0x89,
        0x20, 0xad, 0x16, 0xa1,
        0x67, 0x4e, 0x5d, 0x48)
    );
}
