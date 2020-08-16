//#include <random>
#include <cstdio>

#include "rc6.h"

int main()
{
	vector<u32> plain = { 0x00000000, 0x00000000, 0x00000000, 0x00000000 };
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
		0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00,  
		0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00};
	printf("KEY     : %02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x "
			"%02x %02x %02x %02x\n", 
			key[0], key[1], key[2], key[3], 
			key[4], key[5], key[6], key[7], 
			key[8], key[9], key[10], key[11], 
			key[12], key[13], key[14], key[15]);

	RC6<u32>::encrypt(plain, key);
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
	RC6<u32>::decrypt(plain, key);
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
