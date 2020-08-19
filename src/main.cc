#include <cstdio>

#include "rc6.h"

int main()
{
    RC6<u32> rc6 = RC6<u32>();
    
    printf("========== TEST VECTOR #1 ==========\n");
    vector<u8> key = {        
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
    };
    vector<u8> plain = {
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
    };
    printf("PLAIN   : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    printf("KEY     : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n",
			key[0], key[1], key[2], key[3], 
			key[4], key[5], key[6], key[7], 
			key[8], key[9], key[10], key[11], 
			key[12], key[13], key[14], key[15]);
	rc6.encrypt(plain, key);
    printf("ENCRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    rc6.decrypt(plain, key);
	printf("DECRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    
    printf("========== TEST VECTOR #2 ==========\n");
    key = { 
        0x01, 0x23, 0x45, 0x67, 
        0x89, 0xab, 0xcd, 0xef, 
        0x01, 0x12, 0x23, 0x34, 
        0x45, 0x56, 0x67, 0x78 
    };
    plain = { 
        0x02, 0x13, 0x24, 0x35, 
        0x46, 0x57, 0x68, 0x79, 
        0x8a, 0x9b, 0xac, 0xbd, 
        0xce, 0xdf, 0xe0, 0xf1
    };
    printf("PLAIN   : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    printf("KEY     : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n",
			key[0], key[1], key[2], key[3], 
			key[4], key[5], key[6], key[7], 
			key[8], key[9], key[10], key[11], 
			key[12], key[13], key[14], key[15]);
	rc6.encrypt(plain, key);
    printf("ENCRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    rc6.decrypt(plain, key);
	printf("DECRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    
    printf("========== TEST VECTOR #3 ==========\n");
    key = {        
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
    };
    plain = {
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
    };
    printf("PLAIN   : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n",
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    printf("KEY     : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n"
            "                      "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n",
			key[0], key[1], key[2], key[3], 
			key[4], key[5], key[6], key[7], 
			key[8], key[9], key[10], key[11], 
			key[12], key[13], key[14], key[15],
            key[16], key[17], key[18], key[19],
            key[20], key[21], key[22], key[23]);
	rc6.encrypt(plain, key);
    printf("ENCRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    rc6.decrypt(plain, key);
	printf("DECRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    
    printf("========== TEST VECTOR #4 ==========\n");
    key = {
        0x01, 0x23, 0x45, 0x67, 
        0x89, 0xab, 0xcd, 0xef, 
        0x01, 0x12, 0x23, 0x34, 
        0x45, 0x56, 0x67, 0x78, 
        0x89, 0x9a, 0xab, 0xbc,
        0xcd, 0xde, 0xef, 0xf0
    };
    plain = { 
        0x02, 0x13, 0x24, 0x35, 
        0x46, 0x57, 0x68, 0x79, 
        0x8a, 0x9b, 0xac, 0xbd, 
        0xce, 0xdf, 0xe0, 0xf1
    };
    printf("PLAIN   : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n",
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    printf("KEY     : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n"
            "                      "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n",
			key[0], key[1], key[2], key[3], 
			key[4], key[5], key[6], key[7], 
			key[8], key[9], key[10], key[11], 
			key[12], key[13], key[14], key[15],
            key[16], key[17], key[18], key[19],
            key[20], key[21], key[22], key[23]);
	rc6.encrypt(plain, key);
    printf("ENCRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    rc6.decrypt(plain, key);
	printf("DECRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    
    printf("========== TEST VECTOR #5 ==========\n");
    key = { 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
    };
    plain = {
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
    };
	printf("PLAIN   : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
	printf("KEY     : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n"
            "          "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			key[0], key[1], key[2], key[3], 
			key[4], key[5], key[6], key[7], 
			key[8], key[9], key[10], key[11], 
			key[12], key[13], key[14], key[15],
            key[16], key[17], key[18], key[19],
            key[20], key[21], key[22], key[23],
            key[24], key[25], key[26], key[27],
            key[28], key[29], key[30], key[31]
          );
	rc6.encrypt(plain, key);
	printf("ENCRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);

	rc6.decrypt(plain, key);
	printf("DECRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
    
    printf("========== TEST VECTOR #6 ==========\n");
    key = {
		0x01, 0x23, 0x45, 0x67, 
		0x89, 0xab, 0xcd, 0xef,  
		0x01, 0x12, 0x23, 0x34, 
		0x45, 0x56, 0x67, 0x78,
        0x89, 0x9a, 0xab, 0xbc, 
		0xcd, 0xde, 0xef, 0xf0,
        0x10, 0x32, 0x54, 0x76,
        0x98, 0xba, 0xdc, 0xfe
        
    };
    plain = { 
        0x02, 0x13, 0x24, 0x35, 
        0x46, 0x57, 0x68, 0x79, 
        0x8a, 0x9b, 0xac, 0xbd, 
        0xce, 0xdf, 0xe0, 0xf1
    };
	printf("PLAIN   : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
	printf("KEY     : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n"
            "          "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			key[0], key[1], key[2], key[3], 
			key[4], key[5], key[6], key[7], 
			key[8], key[9], key[10], key[11], 
			key[12], key[13], key[14], key[15],
            key[16], key[17], key[18], key[19],
            key[20], key[21], key[22], key[23],
            key[24], key[25], key[26], key[27],
            key[28], key[29], key[30], key[31]
          );
	rc6.encrypt(plain, key);
	printf("ENCRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);

	rc6.decrypt(plain, key);
	printf("DECRYPT : "
            "%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain[0], plain[1], plain[2], plain[3], 
			plain[4], plain[5], plain[6], plain[7], 
			plain[8], plain[9], plain[10], plain[11], 
			plain[12], plain[13], plain[14], plain[15]);
	return 0;
}
