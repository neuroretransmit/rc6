#pragma once

/// Polyval is a little-endian version of GHASH from GCM

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <bits/stdc++.h>

#include "../types.h"
#include "../binops.h"

using std::vector;
using std::string;
using std::stringstream;
using std::hex;
using std::setw;
using std::setfill;

/// Operater for a 64-bit field
class FieldElement64
{
public:
    FieldElement64(u64 e0, u64 e1) :
        e0(e0),
        e1(e1) {}
    
    FieldElement64(const vector<u8>& bytes)
    {
        from_bytes(bytes, 0);
    }
    
    FieldElement64(const vector<u8>& bytes, size_t offset)
    {
        from_bytes(bytes, offset);
    }
   
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

    FieldElement64 operator+(const FieldElement64& rhs) {
        return FieldElement64(e0 ^ rhs.e0, e1 ^ rhs.e1);
    }
    
    FieldElement64 operator*(const FieldElement64& rhs) {
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
        const u64 r2 = (m1 ^ k2) ^ (r1 << 63) ^ (r1 << 62) ^ (r1 << 57) ^ r0 ^ (r0 >> 1) ^ (r0 >> 2) ^ (r0 >> 7);
        const u64 r3 = k1 ^ r1 ^ (r1 >> 1) ^ (r1 >> 2) ^ (r1 >> 7);
        return FieldElement64(r2, r3);
    }
    
private:
    u64 e0;
    u64 e1;
    
    void from_bytes(const vector<u8>& bytes, size_t offset)
    {
        e0 =      ((u64) bytes[offset + 7] << 56)
                | ((u64) bytes[offset + 6] & 0xff) << 48
                | ((u64) bytes[offset + 5] & 0xff) << 40
                | ((u64) bytes[offset + 4] & 0xff) << 32
                | ((u64) bytes[offset + 3] & 0xff) << 24
                | ((u64) bytes[offset + 2] & 0xff) << 16
                | ((u64) bytes[offset + 1] & 0xff) << 8
                | ((u64) bytes[offset    ] & 0xff);

        e1 =      ((u64) bytes[offset + 15] << 56)
                | ((u64) bytes[offset + 14] & 0xff) << 48
                | ((u64) bytes[offset + 13] & 0xff) << 40
                | ((u64) bytes[offset + 12] & 0xff) << 32
                | ((u64) bytes[offset + 11] & 0xff) << 24
                | ((u64) bytes[offset + 10] & 0xff) << 16
                | ((u64) bytes[offset +  9] & 0xff) << 8
                | ((u64) bytes[offset +  8] & 0xff);
    }

};

class Authenticator
{
public:
    virtual vector<u8> digest() = 0;
    virtual void update(const vector<u8>& bytes) = 0;
    virtual void reset() = 0;
};

template<class T> class Polyval : public Authenticator
{
private:
    const size_t BLOCK_SIZE = sizeof(T) * 4;
    FieldElement64 h;
    FieldElement64 s;
    
public:
    Polyval(u64 h0, u64 h1) :
        h(FieldElement64(h0, h1)),
        s(FieldElement64(0L, 0L)) {}
    
    Polyval(u64 h0, u64 h1, u64 s0, u64 s1) :
        h(FieldElement64(h0, h1)),
        s(FieldElement64(s0, s1)) {}
    
    Polyval(const vector<u8>& h) :
        h(FieldElement64(h)),
        s(FieldElement64(0L, 0L)) {}
    
    Polyval(const vector<u8>& h, const vector<u8>& s) :
        h(FieldElement64(h)),
        s(FieldElement64(s)) {}
    
    Polyval(const string& h_hex) :
        h(FieldElement64(h_hex)),
        s(FieldElement64(0L, 0L)) {}
    
    Polyval(const string& h_hex, const string& s_hex) :
        h(FieldElement64(h_hex)),
        s(FieldElement64(s_hex)) {}
    
    vector<u8> digest()
    {
        return s.bytes();
    }
    
    void update(const vector<u8>& bytes)
    {
        const size_t remainder = bytes.size() % BLOCK_SIZE;
        
        for (size_t i = 0; i < bytes.size() - remainder; i+= BLOCK_SIZE) {
            FieldElement64 block_update = FieldElement64(bytes, i);
            update_block(block_update);
        }
        
        if (remainder) {
            vector<u8> block(bytes.begin(), bytes.end() - remainder);
            //memcpy(&block, &bytes, bytes.size() - remainder);
            const FieldElement64 block_update = FieldElement64(block);
            update_block(block_update);
        }
    }
    
    void update(const string& hex_str)
    {
        const size_t remainder = hex_str.size() % (BLOCK_SIZE * 2);
        
        for (size_t i = 0; i < hex_str.size() - remainder; i+= (BLOCK_SIZE * 2)) {
            FieldElement64 block_update = FieldElement64(hex_str.substr(i, i + (BLOCK_SIZE * 2)));
            update_block(block_update);
        }
        
        if (remainder) {
            string pad(BLOCK_SIZE * 2 - remainder, '0');
            const FieldElement64 block_update = FieldElement64(hex_str.substr(hex_str.size() - remainder) + pad);
            update_block(block_update);
        }
    }
    
    void update_block(const FieldElement64& update)
    {
        s = (s + update) * h;
    }
    
    void update_block(const string& update_hex)
    {
        FieldElement64 update_element = FieldElement64(update_hex);
        update_block(update_element);
    }
    
    void reset()
    {
        s = FieldElement64(0L, 0L);
    }
    
    string str()
    {
        return s.str();
    }
};
