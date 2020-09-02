#include "binops.h"

#include "types.h"

u64 rev64(u64 a)
{
    u64 x = a;

    x = ((x >> 16) & 0x0000ffff0000ffffL) | ((x & 0x0000ffff0000ffffL) << 16);
    x = ((x >> 8)  & 0x00ff00ff00ff00ffL) | ((x & 0x00ff00ff00ff00ffL) << 8);
    x = ((x >> 4)  & 0x0f0f0f0f0f0f0f0fL) | ((x & 0x0f0f0f0f0f0f0f0fL) << 4);
    x = ((x >> 2)  & 0x3333333333333333L) | ((x & 0x3333333333333333L) << 2);
    x = ((x >> 1)  & 0x5555555555555555L) | ((x & 0x5555555555555555L) << 1);

    return (x << 32) | (x >> 32);
}

u64 bmul64(u64 a, u64 b)
{
    const u64 a0 = a & 0x1111111111111111L;
    const u64 a1 = a & 0x2222222222222222L;
    const u64 a2 = a & 0x4444444444444444L;
    const u64 a3 = a & 0x8888888888888888L;

    const u64 b0 = b & 0x1111111111111111L;
    const u64 b1 = b & 0x2222222222222222L;
    const u64 b2 = b & 0x4444444444444444L;
    const u64 b3 = b & 0x8888888888888888L;

    const u64 r0 = ((a0 * b0) ^ (a1 * b3) ^ (a2 * b2) ^ (a3 * b1)) & 0x1111111111111111L;
    const u64 r1 = ((a0 * b1) ^ (a1 * b0) ^ (a2 * b3) ^ (a3 * b2)) & 0x2222222222222222L;
    const u64 r2 = ((a0 * b2) ^ (a1 * b1) ^ (a2 * b0) ^ (a3 * b3)) & 0x4444444444444444L;
    const u64 r3 = ((a0 * b3) ^ (a1 * b2) ^ (a2 * b1) ^ (a3 * b0)) & 0x8888888888888888L;

    return r0 | r1 | r2 | r3;
}

u64 mul_reverse_shift1(u64 a, u64 b)
{
    return rev64(bmul64(a, b)) >> 1;
}



