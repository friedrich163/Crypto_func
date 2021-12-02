// aes.c
// @ ZCU102
// Last Modified: 2021/12/02

#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xaes.h"
#include "xparameters.h"
#define len 64


XAes aes;
XAes_Config *aesPtr;
u32 data_length = 8;	// 8words, 32bytes, 256bits
u32 iv_length = 4;		// 4words, 16bytes, 256bits
u32 key_length = 8;

int char2int(char* ptr)
{
	/*
	    change  8 char. to one int. (hex represent)
	    input:  char array pointer which point to first char addr.
	*/
    int val=0;
    for(int i=0; i<8; ++i)
    {
        u32 tmp_val;

        if(ptr[i]>='0' && ptr[i]<='9') tmp_val = ptr[i] - '0';
        else tmp_val = ptr[i] - 'a' + 10;

        val <<= 4;
        val |= tmp_val;
    }
    return val;
}



void char2int_arr(char* charPtr, int* intArr)
{
    for(int i=0, idx=0; i<=len; i=i+8, ++idx)
    {
        intArr[idx] = char2int(&charPtr[i]);
    }
}



int aes_init()
{
	int status = 0;

	aesPtr = XAes_LookupConfig(XPAR_AES_0_DEVICE_ID);

	if (!aesPtr)
	{
	 xil_printf("ERROR: Lookup of accelerator configuration failed.\n\r");
	  return XST_FAILURE;
	}

	status = XAes_CfgInitialize(&aes, aesPtr);
	if (status != XST_SUCCESS)
	{
	 xil_printf("ERROR: Could not initialize accelerator.\n\r");
	  return XST_FAILURE;
	}
	return XST_SUCCESS;
}


void aes_setup(int* i_data, int* key, int* iv)
{
	data_length = XAes_Write_i_data_Words(&aes, 0, i_data, data_length);
	key_length  = XAes_Write_i_key_Words(&aes, 0, key, key_length);
	iv_length   = XAes_Write_i_iv_Words(&aes, 0, iv, iv_length);
}


int aes_enc(int* o_data)
{
	int finish = 0;

	XAes_Set_i_mode(&aes, 1);	//set encryption mode
	XAes_Start(&aes);

	while(!finish)
	{
		if(XAes_IsDone(&aes))
		{
			XAes_Read_o_data_Words(&aes, 0, o_data, data_length);
			finish = 1;
		}
		if(finish)
			xil_printf("Finish encryption \n\n\r");
	}
	return finish;
}

int aes_dec(int* o_data)
{
	int finish = 0;

	XAes_Set_i_mode(&aes, 0);	//set decryption mode
	XAes_Start(&aes);

	while(!finish)
	{
		if(XAes_IsDone(&aes))
		{
			XAes_Read_o_data_Words(&aes, 0, o_data, data_length);
			finish = 1;
		}
		if(finish)
			xil_printf("Finish decryption \n\n\r");
	}
	return finish;
}


int main()
{
	int status;
	int finish = 0;

    print("Start AES\n\r");

    status = aes_init();
	if(status == XST_SUCCESS)
		xil_printf("Successfully initialize AES  \r\n\n");
	else
		xil_printf("Fail to initialize AES  \r\n\n");


	char* key_str;
	char* data_str;
	char* iv_str;


	int* key_int;
	int* data_int;
	int* iv_int;

	int* o_cipherText;
	int* o_plainText;




	key_str = malloc(sizeof(char)*64);
	data_str = malloc(sizeof(char)*64);
	iv_str = malloc(sizeof(char)*32);

	key_int = malloc(sizeof(int)*8);
	data_int = malloc(sizeof(int)*8);
	iv_int = malloc(sizeof(int)*4);

	o_cipherText = malloc(sizeof(int)*8);
	o_plainText  = malloc(sizeof(int)*8);

	for(int i; i<8; i++)
	{
		key_int[i] = 0;
		data_int[i] = 0;
		o_plainText[i] = 0;
		o_cipherText[i] = 0;

		if(i<4)
			iv_int[i] = 0;
	}

	key_str  = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f";
	data_str = "303132333435363738393a3b3c3d3e3f303132333435363738393a3b3c3d3e3f";
	iv_str = "000102030405060708090a0b0c0d0e0f";

	char *keyPtr = &key_str[0];
	char *dataPtr = &data_str[0];
	char *ivPtr = &iv_str[0];


	char2int_arr(keyPtr, &key_int[0]);
	char2int_arr(dataPtr, &data_int[0]);
	char2int_arr(ivPtr, &iv_int[0]);

	xil_printf("plain text \n\r %s", data_str);

	xil_printf("\n\n\r");

	////// Encryption //////

	aes_setup(data_int, key_int,iv_int);
	finish = aes_enc(o_cipherText);
	while(!finish){}
	xil_printf("cipher text \n\r");

	for(int i = 0; i<8; i++)
	{
		xil_printf(" %08x", o_cipherText[i]);
	}
	finish = 0;

	xil_printf("\n\n\r");
	////// Decryption ///////

	aes_setup(o_cipherText, key_int, iv_int);
	finish = aes_dec(o_plainText);
	while(!finish){}

	xil_printf("output plain text \n\r");

	for(int i = 0; i<8; i++)
	{
		xil_printf(" %08x", o_plainText[i]);
	}
	xil_printf("\n\n\r");



    return 0;
}
