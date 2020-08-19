#include <cstdio>

#include "rc6.h"

int main()
{
    RC6<u32> rc6 = RC6<u32>();
	vector<u32> plain = { 0x35241302, 0x79685746, 0xbdac9b8a, 0xf1e0dfce };
	u8* plain_bytes = reinterpret_cast<u8*>(plain.data());
	printf("PLAIN   : %02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			plain_bytes[0], plain_bytes[1], plain_bytes[2], plain_bytes[3], 
			plain_bytes[4], plain_bytes[5], plain_bytes[6], plain_bytes[7], 
			plain_bytes[8], plain_bytes[9], plain_bytes[10], plain_bytes[11], 
			plain_bytes[12], plain_bytes[13], plain_bytes[14], plain_bytes[15]);

	vector<u8> key = {
		0x01, 0x23, 0x45, 0x67, 
		0x89, 0xab, 0xcd, 0xef,  
		0x01, 0x12, 0x23, 0x34, 
		0x45, 0x56, 0x67, 0x78,
        0x89, 0x9a, 0xab, 0xbc, 
		0xcd, 0xde, 0xef, 0xf0,
        0x10, 0x32, 0x54, 0x76,
        0x98, 0xba, 0xdc, 0xfe
        
    };
    
	printf("KEY     : %02x %02x %02x %02x "
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
	u8* encrypted_bytes = reinterpret_cast<u8*>(plain.data());
	printf("ENCRYPT : %02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			encrypted_bytes[0], encrypted_bytes[1], encrypted_bytes[2], encrypted_bytes[3], 
			encrypted_bytes[4], encrypted_bytes[5], encrypted_bytes[6], encrypted_bytes[7], 
			encrypted_bytes[8], encrypted_bytes[9], encrypted_bytes[10], encrypted_bytes[11], 
			encrypted_bytes[12], encrypted_bytes[13], encrypted_bytes[14], encrypted_bytes[15]);

	u8* decrypted_bytes = reinterpret_cast<u8*>(plain.data());
	rc6.decrypt(plain, key);
	printf("DECRYPT : %02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			decrypted_bytes[0], decrypted_bytes[1], decrypted_bytes[2], decrypted_bytes[3], 
			decrypted_bytes[4], decrypted_bytes[5], decrypted_bytes[6], decrypted_bytes[7], 
			decrypted_bytes[8], decrypted_bytes[9], decrypted_bytes[10], decrypted_bytes[11], 
			decrypted_bytes[12], decrypted_bytes[13], decrypted_bytes[14], decrypted_bytes[15]);
	return 0;
}
