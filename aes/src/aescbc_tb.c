
#include "aescbc.h"

#ifndef __SYNTHESIS__
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#endif

void aes(
	int i_mode,
	uint8_t i_data[32],
	uint8_t i_iv[16],
	uint8_t i_key[32],
	uint8_t o_data[32]
	)
	{
	#pragma HLS INTERFACE s_axilite port=i_mode
	#pragma HLS INTERFACE s_axilite port=i_data
	#pragma HLS INTERFACE s_axilite port=i_iv
	#pragma HLS INTERFACE s_axilite port=i_key
	#pragma HLS INTERFACE s_axilite port=o_data

	#pragma HLS INTERFACE s_axilite port=return

		if(i_mode)
		{
			//encrypt
			aescbc(SET_KEY, i_key,  o_data);
			aescbc(SET_IV,  i_iv,   o_data);
			aescbc(RESET,   i_data, o_data);
			aescbc(ENCRYPT, i_data, o_data);
			aescbc(ENCRYPT, &i_data[16], &o_data[16]);
		}
		else
		{
			//decrypt
			aescbc(SET_KEY, i_key,  o_data);
			aescbc(SET_IV,  i_iv,   o_data);
			aescbc(RESET,   i_data, o_data);
			aescbc(DECRYPT, i_data, o_data);
			aescbc(DECRYPT, &i_data[16], &o_data[16]);
		}

	}

#ifndef __SYNTHESIS__
int main() {
	uint8_t key[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
			0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

	uint8_t iv[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

	uint8_t plaintext[32] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
			   	  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		       		  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		    		  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };

	uint8_t ciphertext[32];
	uint8_t o_plaintext[32];
	int i;

	for( i = 0; i<32; i++)
	{
		ciphertext[i] = 0;
		o_plaintext[i] = 0;
	}

	int mode;
	// Test our engine

	printf("origin plaintext: \n\r");
	for( i = 0; i<32; i++)
		printf("%x ", plaintext[i]);

	printf("\n\n\r");
	
	printf("Encryption\n\n\r");
	mode = 0;
	aes(mode, plaintext, iv, key, ciphertext);

	printf("ciphertext: \n\r");
	for( i = 0; i<32; i++)
		printf("%x ", ciphertext[i]);
	

	printf("\n\n\r");
	
	printf("Decryption\n\n\r");
	mode = 1;
	aes(mode, ciphertext, iv, key, o_plaintext);

	printf("output plaintext: \n\r");
	for( i = 0; i<32; i++)
		printf("%x ", o_plaintext[i]);
	printf("\n\n\r");

	for(i = 0; i<32; i++)
	{
		if(o_plaintext[i] != plaintext[i])
			return 1;
	}
	return 0;
}
#endif
