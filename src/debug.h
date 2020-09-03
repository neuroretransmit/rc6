#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "types.h"

using std::cout;
using std::hex;
using std::setfill;
using std::setw;
using std::string;
using std::vector;

void print_bytes(const string& label, const vector<u8>& bytes)
{
    cout << label << "\t: ";
    for (size_t i = 0; i < bytes.size(); i++)
        cout << hex << setfill('0') << setw(2) << (int) bytes[i];
    cout << "\n";
}
