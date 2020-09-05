#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../../types.h"

using std::hex;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;

/// Operater for a 64-bit field
class FieldElement64
{
  public:
    FieldElement64(u64 e0, u64 e1) : e0(e0), e1(e1) {}

    FieldElement64(const vector<u8>& bytes) { from_bytes(bytes, 0); }

    FieldElement64(const vector<u8>& bytes, size_t offset) { from_bytes(bytes, offset); }

    FieldElement64(const string& hex)
    {
        vector<u8> bytes;

        for (size_t i = 0; i < hex.length(); i += 2) {
            string byte_str = hex.substr(i, 2);
            u8 byte = (u8) strtol(byte_str.c_str(), NULL, 16);
            bytes.push_back(byte);
        }

        from_bytes(bytes, 0);
    }

    string str()
    {
        vector<u8> bytes = this->bytes();
        stringstream ss;
        ss << hex;
        for (size_t i = 0; i < bytes.size(); i++)
            ss << setw(2) << setfill('0') << (u64) bytes[i];
        return ss.str();
    }

    vector<u8> bytes()
    {
        vector<u8> result(16);
        u64* tmp = (u64*) result.data();
        tmp[0] = e0;
        tmp[1] = e1;
        return result;
    }

    FieldElement64 operator+(const FieldElement64& rhs) { return FieldElement64(e0 ^ rhs.e0, e1 ^ rhs.e1); }

    FieldElement64 operator*(const FieldElement64& rhs)
    {
        const u64 a0 = e0;
        const u64 a1 = e1;
        const u64 a0_reversed = rev64(a0);
        const u64 a1_reversed = rev64(a1);
        const u64 b0 = rhs.e0;
        const u64 b1 = rhs.e1;
        const u64 b0_reversed = rev64(b0);
        const u64 b1_reversed = rev64(b1);

        const u64 m1 = bmul64(b1, a1);
        const u64 m2 = bmul64(b0, a0) ^ bmul64(b0 ^ b1, a0 ^ a1) ^ bmul64(b1, a1);

        const u64 k0 = mul_reverse_shift1(b0_reversed, a0_reversed);
        const u64 k1 = mul_reverse_shift1(b1_reversed, a1_reversed);
        const u64 k2 = mul_reverse_shift1(b0_reversed ^ b1_reversed, a0_reversed ^ a1_reversed) ^ (k0 ^ k1);

        const u64 r0 = bmul64(b0, a0);
        const u64 r1 = (m2 ^ k0) ^ (r0 << 63) ^ (r0 << 62) ^ (r0 << 57);
        const u64 r2 =
            (m1 ^ k2) ^ (r1 << 63) ^ (r1 << 62) ^ (r1 << 57) ^ r0 ^ (r0 >> 1) ^ (r0 >> 2) ^ (r0 >> 7);
        const u64 r3 = k1 ^ r1 ^ (r1 >> 1) ^ (r1 >> 2) ^ (r1 >> 7);
        return FieldElement64(r2, r3);
    }

  private:
    u64 e0;
    u64 e1;

    u64 rev64(u64 a)
    {
        u64 x = a;

        x = ((x >> 16) & 0x0000ffff0000ffffL) | ((x & 0x0000ffff0000ffffL) << 16);
        x = ((x >> 8) & 0x00ff00ff00ff00ffL) | ((x & 0x00ff00ff00ff00ffL) << 8);
        x = ((x >> 4) & 0x0f0f0f0f0f0f0f0fL) | ((x & 0x0f0f0f0f0f0f0f0fL) << 4);
        x = ((x >> 2) & 0x3333333333333333L) | ((x & 0x3333333333333333L) << 2);
        x = ((x >> 1) & 0x5555555555555555L) | ((x & 0x5555555555555555L) << 1);

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

    u64 mul_reverse_shift1(u64 a, u64 b) { return rev64(bmul64(a, b)) >> 1; }

    void from_bytes(const vector<u8>& bytes, size_t offset)
    {
        e0 = ((u64) bytes[offset + 7] << 56) | ((u64) bytes[offset + 6] & 0xff) << 48 |
             ((u64) bytes[offset + 5] & 0xff) << 40 | ((u64) bytes[offset + 4] & 0xff) << 32 |
             ((u64) bytes[offset + 3] & 0xff) << 24 | ((u64) bytes[offset + 2] & 0xff) << 16 |
             ((u64) bytes[offset + 1] & 0xff) << 8 | ((u64) bytes[offset] & 0xff);

        e1 = ((u64) bytes[offset + 15] << 56) | ((u64) bytes[offset + 14] & 0xff) << 48 |
             ((u64) bytes[offset + 13] & 0xff) << 40 | ((u64) bytes[offset + 12] & 0xff) << 32 |
             ((u64) bytes[offset + 11] & 0xff) << 24 | ((u64) bytes[offset + 10] & 0xff) << 16 |
             ((u64) bytes[offset + 9] & 0xff) << 8 | ((u64) bytes[offset + 8] & 0xff);
    }
};
