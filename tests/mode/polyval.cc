#include <gtest/gtest.h>

#include "mode/polyval.h"

static Polyval POLYVAL_0 = Polyval<u32>(0L, 0L, 0L, 0L);
static Polyval POLYVAL_A = Polyval<u32>(1L, 1L, 1L, 1L);

static const vector<u8> BYTES_A = {
    1, 0, 0 ,0,
    0, 0, 0, 0,
    1, 0, 0, 0,
    0, 0, 0, 0
};

TEST(Polyval, ConstructorFromHLongs)
{
    Polyval ACTUAL = Polyval<u32>(0, 0);
    EXPECT_EQ(POLYVAL_0.str(), ACTUAL.str());
}

TEST(Polyval, ConstructorFromHAndSLongs)
{
    Polyval ACTUAL = Polyval<u32>(0, 0, 0, 0);
    EXPECT_EQ(POLYVAL_0.str(), ACTUAL.str());
}

TEST(Polyval, ConstructFromOwnDigest)
{
    Polyval ACTUAL = Polyval<u32>(BYTES_A, POLYVAL_A.digest());
    EXPECT_EQ(POLYVAL_A.str(), ACTUAL.str());
}

TEST(Polyval, ConstructorFromHBytes)
{
    Polyval ACTUAL = Polyval<u32>(BYTES_A);
    EXPECT_EQ(POLYVAL_0.str(), ACTUAL.str());
}

TEST(Polyval, ConstructorFromHAndSBytes)
{
    Polyval ACTUAL = Polyval<u32>(BYTES_A, BYTES_A);
    EXPECT_EQ(POLYVAL_A.str(), ACTUAL.str());
}

TEST(Polyval, ConstructorFromHString)
{
    Polyval ACTUAL = Polyval<u32>("00000000000000000000000000000000");
    EXPECT_EQ(POLYVAL_0.str(), ACTUAL.str());
}

TEST(Polyval, ConstructorFromHAndSStrings)
{
    Polyval ACTUAL = Polyval<u32>("00000000000000000000000000000000", "00000000000000000000000000000000");
    EXPECT_EQ(POLYVAL_0.str(), ACTUAL.str());
}

TEST(Polyval, UpdateBlock0)
{
    Polyval ACTUAL = Polyval<u32>("25629347589242761d31f826ba4b757b");
    ACTUAL.update_block("00000000000000000000000000000000");
    EXPECT_EQ("00000000000000000000000000000000", ACTUAL.str());
}

TEST(Polyval, UpdateBlock)
{
    Polyval ACTUAL = Polyval<u32>("25629347589242761d31f826ba4b757b");
    ACTUAL.update_block("4f4f95668c83dfb6401762bb2d01a262");
    EXPECT_EQ("cedac64537ff50989c16011551086d77", ACTUAL.str());
    ACTUAL.update_block("d1a24ddd2721d006bbe45f20d3c9f362");
    EXPECT_EQ("f7a3b47b846119fae5b7866cf5e5b77e", ACTUAL.str());
}

TEST(Polyval, UpdateNotDependingOnZeroBlock)
{
    Polyval ACTUAL = Polyval<u32>("25629347589242761d31f826ba4b757b");
    ACTUAL.update("000000000000000000000000000000004f4f95668c83dfb6401762bb2d01a262d1a24ddd2721d006bbe45f20d3c9f362");
    EXPECT_EQ("f7a3b47b846119fae5b7866cf5e5b77e", ACTUAL.str());
}

TEST(Polyval, Update)
{
    Polyval ACTUAL = Polyval<u32>("25629347589242761d31f826ba4b757b");
    ACTUAL.update("4f4f95668c83dfb6401762bb2d01a262d1a24ddd2721d006bbe45f20d3c9f362");
    EXPECT_EQ("f7a3b47b846119fae5b7866cf5e5b77e", ACTUAL.str());
}

TEST(Polyval, PadWithZeroes)
{
    Polyval p0 = Polyval<u32>("25629347589242761d31f826ba4b757b");
    p0.update("d1a24ddd2721d006bbe45f20d3c9f36200");
    Polyval p1 = Polyval<u32>("25629347589242761d31f826ba4b757b");
    p1.update("d1a24ddd2721d006bbe45f20d3c9f36200000000000000000000000000000000");
    EXPECT_EQ(p0.str(), p1.str());
}

TEST(Polyval, ResetSToZero)
{
    Polyval ACTUAL = Polyval<u32>(1L, 1L);
    ACTUAL.reset();
    EXPECT_EQ("00000000000000000000000000000000", ACTUAL.str());
    
}
