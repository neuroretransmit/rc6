#pragma once

/// Polyval is a little-endian version of GHASH from GCM

#include <bits/stdc++.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "fieldelement64.h"

using std::reverse;
using std::string;
using std::vector;

/// Abstract authenticator
class Authenticator
{
  public:
    virtual vector<u8> digest() = 0;
    virtual void update(const vector<u8> &bytes) = 0;
    virtual void reset() = 0;
};

/// Polyval authenticator
template <class T> class Polyval : public Authenticator
{
  private:
    const size_t BLOCK_SIZE = sizeof(T) * 4;
    FieldElement64 h;
    FieldElement64 s;

  public:
    Polyval(u64 h0, u64 h1) : h(FieldElement64(h0, h1)), s(FieldElement64(0L, 0L)) {}

    Polyval(u64 h0, u64 h1, u64 s0, u64 s1) : h(FieldElement64(h0, h1)), s(FieldElement64(s0, s1)) {}

    Polyval(const vector<u8> &h) : h(FieldElement64(h)), s(FieldElement64(0L, 0L)) {}

    Polyval(const vector<u8> &h, const vector<u8> &s) : h(FieldElement64(h)), s(FieldElement64(s)) {}

    Polyval(const string &h_hex) : h(FieldElement64(h_hex)), s(FieldElement64(0L, 0L)) {}

    Polyval(const string &h_hex, const string &s_hex) : h(FieldElement64(h_hex)), s(FieldElement64(s_hex)) {}

    vector<u8> digest() { return s.bytes(); }

    void update(const vector<u8> &bytes)
    {
        const size_t remainder = 0 ? bytes.size() < BLOCK_SIZE : bytes.size() % BLOCK_SIZE;

        for (size_t i = 0; i < bytes.size() - remainder; i += BLOCK_SIZE) {
            FieldElement64 block_update = FieldElement64(bytes, i);
            update_block(block_update);
        }

        if (remainder) {
            vector<u8> block(bytes.begin(), bytes.end() - remainder);
            const FieldElement64 block_update = FieldElement64(block);
            update_block(block_update);
        }
    }

    void update(const string &hex_str)
    {
        const size_t remainder = hex_str.size() % (BLOCK_SIZE * 2);

        for (size_t i = 0; i < hex_str.size() - remainder; i += (BLOCK_SIZE * 2)) {
            FieldElement64 block_update = FieldElement64(hex_str.substr(i, i + (BLOCK_SIZE * 2)));
            update_block(block_update);
        }

        if (remainder) {
            string pad(BLOCK_SIZE * 2 - remainder, '0');
            const FieldElement64 block_update =
                FieldElement64(hex_str.substr(hex_str.size() - remainder) + pad);
            update_block(block_update);
        }
    }

    void update_block(const FieldElement64 &update) { s = (s + update) * h; }

    void update_block(const string &update_hex)
    {
        FieldElement64 update_element = FieldElement64(update_hex);
        update_block(update_element);
    }

    void reset() { s = FieldElement64(0L, 0L); }

    string str() { return s.str(); }
};
