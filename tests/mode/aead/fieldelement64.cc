#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

#include "rc6/mode/aead/polyval.h"

using namespace ::testing;
using std::cout;

static FieldElement64 ELEMENT_A = FieldElement64(1L, 1L);
static const FieldElement64 ELEMENT_B = FieldElement64(2L, 2L);
static const vector<u8> BYTES_A = {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
static const vector<u8> BYTES = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};

TEST(FieldElement64, ConstructorFromString)
{
    FieldElement64 ACTUAL = FieldElement64("01000000000000000100000000000000");
    ACTUAL = FieldElement64(BYTES_A);
    EXPECT_EQ(ELEMENT_A.str(), ACTUAL.str());
}

TEST(FieldElement64, ConstructorFromBytes)
{
    FieldElement64 ACTUAL = FieldElement64(BYTES_A);
    ACTUAL = FieldElement64(BYTES_A);
    EXPECT_EQ(ELEMENT_A.str(), ACTUAL.str());
}

TEST(FieldElement64, ConstructorFromBytesWithOffset)
{
    FieldElement64 ACTUAL = FieldElement64(BYTES, 4);
    EXPECT_EQ(ELEMENT_A.str(), ACTUAL.str());
}

TEST(FieldElement64, ConstructorFromLongs)
{
    FieldElement64 ACTUAL = FieldElement64(0x1L, 0x1L);
    ACTUAL = FieldElement64(BYTES_A);
    EXPECT_EQ(ELEMENT_A.str(), ACTUAL.str());
}

TEST(FieldElement64, str) { EXPECT_EQ("01000000000000000100000000000000", ELEMENT_A.str()); }

TEST(FieldElement64, bytes) { EXPECT_EQ(BYTES_A, ELEMENT_A.bytes()); }

TEST(FieldElement64, OperatorAdd)
{
    EXPECT_EQ("03000000000000000300000000000000", (ELEMENT_A + ELEMENT_B).str());
}

TEST(FieldElement64, OperatorMul)
{
    EXPECT_EQ("010000000000000000000000000008e6", (ELEMENT_A * ELEMENT_B).str());
}
