// ecc_dec.c
// @ ZCU102
// Last Modified: 2021/11/29

#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xecc_dec.h"
#include "xparameters.h"
#define len 64


XEcc_dec ecc_dec;
XEcc_dec_Config *ecc_decPtr;

int char2int(char* ptr)
{
	/*
	    change  8 char. to one int. (hex represent)
	    input:  char array pointer which point to first char addr.
	*/
    u32 val=0;
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

void char2int_arr(char* charPtr, u32* intArr)
{
    for(int i=len-8, idx=0; i>=0; i=i-8, ++idx)
    {
        intArr[idx] = char2int(&charPtr[i]);
    }
}


int ecc_decrypt_init()
{
	int status = 0;

	ecc_decPtr = XEcc_dec_LookupConfig(XPAR_ECC_DEC_0_DEVICE_ID);

	if (!ecc_decPtr)
	{
	 xil_printf("ERROR: Lookup of accelerator configuration failed.\n\r");
	  return XST_FAILURE;
	}

	status = XEcc_dec_CfgInitialize(&ecc_dec, ecc_decPtr);
	if (status != XST_SUCCESS)
	{
	 xil_printf("ERROR: Could not initialize accelerator.\n\r");
	  return XST_FAILURE;
	}
	return XST_SUCCESS;
}

void ecc_decrypt_setup(u32* C1x, u32* C1y, u32* C2x, u32* C2y, u32* privKey)
{
	XEcc_dec_C1x_v i_C1x;
	XEcc_dec_C1y_v i_C1y;
	XEcc_dec_C2x_v i_C2x;
	XEcc_dec_C2y_v i_C2y;

	XEcc_dec_Privatekey_v i_privKey;

	i_C1x.word_0 = C1x[0];
	i_C1x.word_1 = C1x[1];
	i_C1x.word_2 = C1x[2];
	i_C1x.word_3 = C1x[3];
	i_C1x.word_4 = C1x[4];
	i_C1x.word_5 = C1x[5];
	i_C1x.word_6 = C1x[6];
	i_C1x.word_7 = C1x[7];

	i_C1y.word_0 = C1y[0];
	i_C1y.word_1 = C1y[1];
	i_C1y.word_2 = C1y[2];
	i_C1y.word_3 = C1y[3];
	i_C1y.word_4 = C1y[4];
	i_C1y.word_5 = C1y[5];
	i_C1y.word_6 = C1y[6];
	i_C1y.word_7 = C1y[7];

	i_C2x.word_0 = C2x[0];
	i_C2x.word_1 = C2x[1];
	i_C2x.word_2 = C2x[2];
	i_C2x.word_3 = C2x[3];
	i_C2x.word_4 = C2x[4];
	i_C2x.word_5 = C2x[5];
	i_C2x.word_6 = C2x[6];
	i_C2x.word_7 = C2x[7];

	i_C2y.word_0 = C2y[0];
	i_C2y.word_1 = C2y[1];
	i_C2y.word_2 = C2y[2];
	i_C2y.word_3 = C2y[3];
	i_C2y.word_4 = C2y[4];
	i_C2y.word_5 = C2y[5];
	i_C2y.word_6 = C2y[6];
	i_C2y.word_7 = C2y[7];

	i_privKey.word_0 = privKey[0];
	i_privKey.word_1 = privKey[1];
	i_privKey.word_2 = privKey[2];
	i_privKey.word_3 = privKey[3];
	i_privKey.word_4 = privKey[4];
	i_privKey.word_5 = privKey[5];
	i_privKey.word_6 = privKey[6];
	i_privKey.word_7 = privKey[7];

	XEcc_dec_Set_C1x_V(&ecc_dec, i_C1x);
	XEcc_dec_Set_C1y_V(&ecc_dec, i_C1y);
	XEcc_dec_Set_C2x_V(&ecc_dec, i_C2x);
	XEcc_dec_Set_C2y_V(&ecc_dec, i_C2y);

	XEcc_dec_Set_privateKey_V(&ecc_dec, i_privKey);

}


int ecc_decrypt(u32* PMx, u32* PMy)
{
	XEcc_dec_Pmx_v o_PMx;
	XEcc_dec_Pmy_v o_PMy;

	int dec_finish = 0;

	xil_printf("Start Decrypting \n\r");
	XEcc_dec_Start(&ecc_dec);

	while(!dec_finish)
	{
		if(XEcc_dec_IsDone(&ecc_dec))
		{
			xil_printf("...\r\n");
			o_PMx = XEcc_dec_Get_PMx_V(&ecc_dec);
			o_PMy = XEcc_dec_Get_PMy_V(&ecc_dec);

			PMx[0] = o_PMx.word_7;
			PMx[1] = o_PMx.word_6;
			PMx[2] = o_PMx.word_5;
			PMx[3] = o_PMx.word_4;
			PMx[4] = o_PMx.word_3;
			PMx[5] = o_PMx.word_2;
			PMx[6] = o_PMx.word_1;
			PMx[7] = o_PMx.word_0;

			PMy[0] = o_PMy.word_7;
			PMy[1] = o_PMy.word_6;
			PMy[2] = o_PMy.word_5;
			PMy[3] = o_PMy.word_4;
			PMy[4] = o_PMy.word_3;
			PMy[5] = o_PMy.word_2;
			PMy[6] = o_PMy.word_1;
			PMy[7] = o_PMy.word_0;

			dec_finish = 1;
		}
		if(dec_finish)
		{
			xil_printf("Finish decryption \n\n\r");

		}
	}
	return dec_finish;
}

int main()
{
   xil_printf("Start ECC Encrypt Project\n\n\r");

   int status;
   int dec_finish = 0;

   char* C1x_str;
   char* C1y_str;
   char* C2x_str;
   char* C2y_str;
   char* privKey_str;

   u32* C1x_int;
   u32* C1y_int;
   u32* C2x_int;
   u32* C2y_int;
   u32* privKey_int;

   u32* PMx;
   u32* PMy;

   C1x_str = malloc(sizeof(char)*64);
   C1y_str = malloc(sizeof(char)*64);
   C2x_str = malloc(sizeof(char)*64);
   C2y_str = malloc(sizeof(char)*64);
   privKey_str = malloc(sizeof(char)*64);

   C1x_int = malloc(sizeof(int)*8);
   C1y_int = malloc(sizeof(int)*8);
   C2x_int = malloc(sizeof(int)*8);
   C2y_int = malloc(sizeof(int)*8);
   privKey_int = malloc(sizeof(int)*8);


   PMx = malloc(sizeof(int)*8);
   PMy = malloc(sizeof(int)*8);

   for(int i = 0; i<8; i++)
   {
	   C1x_int[i] = 0;	C1y_int[i] = 0;
	   C2x_int[i] = 0;	C2y_int[i] = 0;
	   privKey_int[i] = 0;
	   PMx[i] = 0;	PMy[i] = 0;
   }

   C1x_str = "74bc99f4a27041c593b5363c60d870ffd4b0b7cee1d7ec10904df7d843a1a5bf";
   C1y_str = "06c8b081f3a592ca35c27b5dfd0c1807b2cf697dc9493106816e98bb1886e321";
   C2x_str = "56565a7b67af1245dc87f8773d7c36a35641eadc217f4f4f6014cd9f70f5b79c";
   C2y_str = "2ca45c311a9d7812f0652e1823047d28add106b03190df4116bcaae181a2228a";

   privKey_str = "90f45561b5a43da27f35706348bf86a4759b238a58a0eddb242aa264d0f02f55";

	///////////// Initialization /////////////

	status = ecc_decrypt_init();
	if(status == XST_SUCCESS)
		xil_printf("Successfully initialize ECC Encrypt \r\n\n");
	else
		xil_printf("Fail to initialize ECC Encrypt \r\n\n");

	///////////// Decrypt /////////////

	char *C1xPtr = &C1x_str[0];
	char *C1yPtr = &C1y_str[0];
	char *C2xPtr = &C2x_str[0];
	char *C2yPtr = &C2y_str[0];
	char *privKeyPtr = &privKey_str[0];

	char2int_arr(C1xPtr, &C1x_int[0]);
	char2int_arr(C1yPtr, &C1y_int[0]);
	char2int_arr(C2xPtr, &C2x_int[0]);
	char2int_arr(C2yPtr, &C2y_int[0]);
	char2int_arr(privKeyPtr, &privKey_int[0]);



	xil_printf("Cipher Text: \n\r");
	xil_printf("%s \n\r", C1x_str);
	xil_printf("%s \n\n\r", C1y_str);
	xil_printf("%s \n\r", C2x_str);
	xil_printf("%s \n\n\r", C2y_str);

	xil_printf("private key: \n\r");
	xil_printf("%s \n\n\r", privKey_str);

	ecc_decrypt_setup(C1x_int, C1y_int, C2x_int, C2y_int, privKey_int);
	dec_finish = ecc_decrypt(PMx, PMy);

	while(!dec_finish){}

	if(dec_finish)
	{

	xil_printf("Plain Text: \n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", PMx[i]);
	xil_printf("\n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", PMy[i]);
	xil_printf("\n\n\r");
	}

	free(C1x_str);
	free(C1y_str);
	free(C2x_str);
	free(C2y_str);

	free(C1x_int);
	free(C1y_int);
	free(C2x_int);
	free(C2y_int);

	free(privKey_int);
	free(privKey_str);

   return 0;
}
