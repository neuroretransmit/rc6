#include <gtest/gtest.h>

#include "rc6/binops.h"

TEST(BinOps, ROL)
{
    u32 actual = rol(0b00000001u, 3);
    u32 expected = 0b00001000u;
    ASSERT_EQ(actual, expected);
}

TEST(BinOps, ROR)
{
    u32 actual = ror(0b00001000u, 3);
    u32 expected = 0b00000001u;
    ASSERT_EQ(actual, expected);
}

TEST(BinOps, IsBigEndian)
{
    bool actual = is_big_endian();
    ASSERT_EQ(false, actual);
}
