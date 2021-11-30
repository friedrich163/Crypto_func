// ecc_dec.c
// @ ZCU102
// Last Modified: 2021/11/30

#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xecc_enc_dec.h"
#include "xparameters.h"
#define len 64


XEcc_enc_dec ecc_enc_dec;
XEcc_enc_dec_Config *ecc_enc_decPtr;

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


int ecc_init()
{
	int status = 0;

	ecc_enc_decPtr = XEcc_enc_dec_LookupConfig(XPAR_ECC_ENC_DEC_0_DEVICE_ID);

	if (!ecc_enc_decPtr)
	{
	 xil_printf("ERROR: Lookup of accelerator configuration failed.\n\r");
	  return XST_FAILURE;
	}

	status = XEcc_enc_dec_CfgInitialize(&ecc_enc_dec, ecc_enc_decPtr);
	if (status != XST_SUCCESS)
	{
	 xil_printf("ERROR: Could not initialize accelerator.\n\r");
	  return XST_FAILURE;
	}
	return XST_SUCCESS;
}

void ecc_encrypt_setup( u32* Px, u32* Py, u32* PMx, u32* PMy )
{
	XEcc_enc_dec_I_px_c1x_v i_Px;
	XEcc_enc_dec_I_py_c1y_v i_Py;
	XEcc_enc_dec_I_pmx_c2x_v i_PMx;
	XEcc_enc_dec_I_pmy_c2y_v i_PMy;
	XEcc_enc_dec_I_randomkey_privkey_v i_randomKey;


	char randomKey_str[64] = "2222222222222222222222222222222222222222222222211111111111111111";
	char *randomKeyPtr = &randomKey_str[0];
	u32 randomKey[8];

	char2int_arr(randomKeyPtr, &randomKey[0]);

	i_randomKey.word_0 = randomKey[0];
	i_randomKey.word_1 = randomKey[1];
	i_randomKey.word_2 = randomKey[2];
	i_randomKey.word_3 = randomKey[3];
	i_randomKey.word_4 = randomKey[4];
	i_randomKey.word_5 = randomKey[5];
	i_randomKey.word_6 = randomKey[6];
	i_randomKey.word_7 = randomKey[7];

	i_Px.word_0 = Px[0];
	i_Px.word_1 = Px[1];
	i_Px.word_2 = Px[2];
	i_Px.word_3 = Px[3];
	i_Px.word_4 = Px[4];
	i_Px.word_5 = Px[5];
	i_Px.word_6 = Px[6];
	i_Px.word_7 = Px[7];

	i_Py.word_0 = Py[0];
	i_Py.word_1 = Py[1];
	i_Py.word_2 = Py[2];
	i_Py.word_3 = Py[3];
	i_Py.word_4 = Py[4];
	i_Py.word_5 = Py[5];
	i_Py.word_6 = Py[6];
	i_Py.word_7 = Py[7];

	i_PMx.word_0 = PMx[0];
	i_PMx.word_1 = PMx[1];
	i_PMx.word_2 = PMx[2];
	i_PMx.word_3 = PMx[3];
	i_PMx.word_4 = PMx[4];
	i_PMx.word_5 = PMx[5];
	i_PMx.word_6 = PMx[6];
	i_PMx.word_7 = PMx[7];

	i_PMy.word_0 = PMy[0];
	i_PMy.word_1 = PMy[1];
	i_PMy.word_2 = PMy[2];
	i_PMy.word_3 = PMy[3];
	i_PMy.word_4 = PMy[4];
	i_PMy.word_5 = PMy[5];
	i_PMy.word_6 = PMy[6];
	i_PMy.word_7 = PMy[7];

	XEcc_enc_dec_Set_i_mode(&ecc_enc_dec, 1);	//Set encryption mode

	XEcc_enc_dec_Set_i_Px_C1x_V(&ecc_enc_dec, i_Px);
	XEcc_enc_dec_Set_i_Py_C1y_V(&ecc_enc_dec, i_Py);

	XEcc_enc_dec_Set_i_PMx_C2x_V(&ecc_enc_dec, i_PMx);
	XEcc_enc_dec_Set_i_PMy_C2y_V(&ecc_enc_dec, i_PMy);
	XEcc_enc_dec_Set_i_randomKey_privKey_V(&ecc_enc_dec, i_randomKey);

}

int ecc_encrypt(u32* C1x, u32* C1y, u32* C2x, u32* C2y)
{
	XEcc_enc_dec_O_c1x_pmx_v o_C1x;
	XEcc_enc_dec_O_c1y_pmy_v o_C1y;
	XEcc_enc_dec_O_c2x_v o_C2x;
	XEcc_enc_dec_O_c2y_v o_C2y;

	int enc_finish = 0;

	xil_printf("Start Encrypting \n\r");
	XEcc_enc_dec_Start(&ecc_enc_dec);

	while(!enc_finish)
	{
		while(XEcc_enc_dec_IsDone(&ecc_enc_dec))
		{
			 xil_printf("... \r\n");
			 o_C1x = XEcc_enc_dec_Get_o_C1x_PMx_V(&ecc_enc_dec);
			 o_C2x = XEcc_enc_dec_Get_o_C2x_V(&ecc_enc_dec);
			 o_C1y = XEcc_enc_dec_Get_o_C1y_PMy_V(&ecc_enc_dec);
			 o_C2y = XEcc_enc_dec_Get_o_C2y_V(&ecc_enc_dec);

			 C1x[0] = o_C1x.word_7;
			 C1x[1] = o_C1x.word_6;
			 C1x[2] = o_C1x.word_5;
			 C1x[3] = o_C1x.word_4;
			 C1x[4] = o_C1x.word_3;
			 C1x[5] = o_C1x.word_2;
			 C1x[6] = o_C1x.word_1;
			 C1x[7] = o_C1x.word_0;

			 C1y[0] = o_C1y.word_7;
			 C1y[1] = o_C1y.word_6;
			 C1y[2] = o_C1y.word_5;
			 C1y[3] = o_C1y.word_4;
			 C1y[4] = o_C1y.word_3;
			 C1y[5] = o_C1y.word_2;
			 C1y[6] = o_C1y.word_1;
			 C1y[7] = o_C1y.word_0;

			 C2x[0] = o_C2x.word_7;
			 C2x[1] = o_C2x.word_6;
			 C2x[2] = o_C2x.word_5;
			 C2x[3] = o_C2x.word_4;
			 C2x[4] = o_C2x.word_3;
			 C2x[5] = o_C2x.word_2;
			 C2x[6] = o_C2x.word_1;
			 C2x[7] = o_C2x.word_0;

			 C2y[0] = o_C2y.word_7;
			 C2y[1] = o_C2y.word_6;
			 C2y[2] = o_C2y.word_5;
			 C2y[3] = o_C2y.word_4;
			 C2y[4] = o_C2y.word_3;
			 C2y[5] = o_C2y.word_2;
			 C2y[6] = o_C2y.word_1;
			 C2y[7] = o_C2y.word_0;

			 enc_finish = 1;

		}

		if(enc_finish)
		{
			 xil_printf("Finish encryption \n\n\r");
		}

	}
	return enc_finish;

}


void ecc_decrypt_setup(u32* C1x, u32* C1y, u32* C2x, u32* C2y, u32* privKey)
{
	XEcc_enc_dec_I_px_c1x_v i_C1x;
	XEcc_enc_dec_I_py_c1y_v i_C1y;
	XEcc_enc_dec_I_pmx_c2x_v i_C2x;
	XEcc_enc_dec_I_pmy_c2y_v i_C2y;

	XEcc_enc_dec_I_randomkey_privkey_v i_privKey;

	i_C1x.word_0 = C1x[7];
	i_C1x.word_1 = C1x[6];
	i_C1x.word_2 = C1x[5];
	i_C1x.word_3 = C1x[4];
	i_C1x.word_4 = C1x[3];
	i_C1x.word_5 = C1x[2];
	i_C1x.word_6 = C1x[1];
	i_C1x.word_7 = C1x[0];

	i_C1y.word_0 = C1y[7];
	i_C1y.word_1 = C1y[6];
	i_C1y.word_2 = C1y[5];
	i_C1y.word_3 = C1y[4];
	i_C1y.word_4 = C1y[3];
	i_C1y.word_5 = C1y[2];
	i_C1y.word_6 = C1y[1];
	i_C1y.word_7 = C1y[0];

	i_C2x.word_0 = C2x[7];
	i_C2x.word_1 = C2x[6];
	i_C2x.word_2 = C2x[5];
	i_C2x.word_3 = C2x[4];
	i_C2x.word_4 = C2x[3];
	i_C2x.word_5 = C2x[2];
	i_C2x.word_6 = C2x[1];
	i_C2x.word_7 = C2x[0];

	i_C2y.word_0 = C2y[7];
	i_C2y.word_1 = C2y[6];
	i_C2y.word_2 = C2y[5];
	i_C2y.word_3 = C2y[4];
	i_C2y.word_4 = C2y[3];
	i_C2y.word_5 = C2y[2];
	i_C2y.word_6 = C2y[1];
	i_C2y.word_7 = C2y[0];

	i_privKey.word_0 = privKey[0];
	i_privKey.word_1 = privKey[1];
	i_privKey.word_2 = privKey[2];
	i_privKey.word_3 = privKey[3];
	i_privKey.word_4 = privKey[4];
	i_privKey.word_5 = privKey[5];
	i_privKey.word_6 = privKey[6];
	i_privKey.word_7 = privKey[7];

	XEcc_enc_dec_Set_i_mode(&ecc_enc_dec, 0);	//Set decryption mode

	XEcc_enc_dec_Set_i_Px_C1x_V(&ecc_enc_dec, i_C1x);
	XEcc_enc_dec_Set_i_Py_C1y_V(&ecc_enc_dec, i_C1y);
	XEcc_enc_dec_Set_i_PMx_C2x_V(&ecc_enc_dec, i_C2x);
	XEcc_enc_dec_Set_i_PMy_C2y_V(&ecc_enc_dec, i_C2y);

	XEcc_enc_dec_Set_i_randomKey_privKey_V(&ecc_enc_dec, i_privKey);

}


int ecc_decrypt(u32* PMx, u32* PMy)
{
	XEcc_enc_dec_O_c1x_pmx_v o_PMx;
	XEcc_enc_dec_O_c1y_pmy_v o_PMy;

	int dec_finish = 0;

	xil_printf("Start Decrypting \n\r");
	XEcc_enc_dec_Start(&ecc_enc_dec);

	while(!dec_finish)
	{
		if(XEcc_enc_dec_IsDone(&ecc_enc_dec))
		{
			xil_printf("...\r\n");
			o_PMx = XEcc_enc_dec_Get_o_C1x_PMx_V(&ecc_enc_dec);
			o_PMy = XEcc_enc_dec_Get_o_C1y_PMy_V(&ecc_enc_dec);

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



int compareArray(u32* PMx_int, u32* PMy_int, u32* o_PMx, u32* o_PMy)
{
	for(int i = 0; i<8; i++)
	{
		if(PMx_int[7-i] != o_PMx[i] || PMy_int[7-i] != o_PMy[i])
		{
			return 0;
		}
	}
	return 1;
}


int main()
{
   xil_printf("Start ECC Encrypt/Decrypt Project\n\n\r");

   int status;
   int dec_finish = 0;
   int enc_finish = 0;
   int result = 0;

   char* privKey_str;

   u32* C1x_int;
   u32* C1y_int;
   u32* C2x_int;
   u32* C2y_int;
   u32* privKey_int;

   u32* o_PMx;
   u32* o_PMy;

   char* Px_str;
   char* Py_str;
   char* PMx_str;
   char* PMy_str;

   u32* Px_int;
   u32* Py_int;
   u32* PMx_int;
   u32* PMy_int;

   privKey_str = malloc(sizeof(char)*64);


   Px_str = malloc(sizeof(char)*64);
   Py_str = malloc(sizeof(char)*64);
   PMx_str = malloc(sizeof(char)*64);
   PMy_str = malloc(sizeof(char)*64);

   Px_int = malloc(sizeof(int)*8);
   Py_int = malloc(sizeof(int)*8);
   PMx_int = malloc(sizeof(int)*8);
   PMy_int = malloc(sizeof(int)*8);

   C1x_int = malloc(sizeof(int)*8);
   C1y_int = malloc(sizeof(int)*8);
   C2x_int = malloc(sizeof(int)*8);
   C2y_int = malloc(sizeof(int)*8);

   o_PMx = malloc(sizeof(int)*8);
   o_PMy = malloc(sizeof(int)*8);

   privKey_int = malloc(sizeof(int)*8);

   for(int i = 0; i<8; i++)
   {
	   C1x_int[i] = 0;	C1y_int[i] = 0;
	   C2x_int[i] = 0;	C2y_int[i] = 0;
	   privKey_int[i] = 0;
	   Px_int[i] = 0;	PMx_int[i] = 0;
	   Py_int[i] = 0;	PMy_int[i] = 0;
	   o_PMx[i] = 0;	o_PMy[i] = 0;
   }


   Px_str = "f256c6e6c80b213f2aa0b01744235d515c594435be651b15883a10dd472fa646";
   Py_str = "ce62eaf3670dc5cb9100a0ca2a55b2c147c1e9a38ce428878e7d46e1fb714a99";
   PMx_str = "4041424344454647484940414243444546474849404142434445464748494041";
   PMy_str = "3031323334353637383930313233343536373839303132333435363738393031";

   privKey_str = "90f45561b5a43da27f35706348bf86a4759b238a58a0eddb242aa264d0f02f55";

	///////////// Initialization /////////////

	status = ecc_init();
	if(status == XST_SUCCESS)
		xil_printf("Successfully initialize ECC  \r\n\n");
	else
		xil_printf("Fail to initialize ECC  \r\n\n");

	///////////// Encrypt /////////////

	char *PxPtr = &Px_str[0];
	char *PyPtr = &Py_str[0];
	char *PMxPtr = &PMx_str[0];
	char *PMyPtr = &PMy_str[0];

	char2int_arr(PxPtr, &Px_int[0]);
	char2int_arr(PyPtr, &Py_int[0]);
	char2int_arr(PMxPtr, &PMx_int[0]);
	char2int_arr(PMyPtr, &PMy_int[0]);

	xil_printf("plain text: \n\r");
	xil_printf("%s \n\r", PMx_str);
	xil_printf("%s \n\n\r", PMy_str);

	xil_printf("public key: \n\r");
	xil_printf("%s \n\r", Px_str);
	xil_printf("%s \n\n\r", Py_str);

	ecc_encrypt_setup(Px_int, Py_int, PMx_int, PMy_int);
	enc_finish = ecc_encrypt(C1x_int, C1y_int, C2x_int, C2y_int);



	while(!enc_finish){}

	///////////// Decrypt /////////////


	char *privKeyPtr = &privKey_str[0];


	char2int_arr(privKeyPtr, &privKey_int[0]);

	ecc_decrypt_setup(C1x_int, C1y_int, C2x_int, C2y_int, privKey_int);
	dec_finish = ecc_decrypt(o_PMx, o_PMy);

	xil_printf("private key: \n\r");
	xil_printf("%s \n\n\r", privKey_str);


	while(!dec_finish){}

	if(dec_finish)
	{
		xil_printf("Plain Text: \n\r");
		for(int i = 0; i<8; i++)
			xil_printf("%08x", o_PMx[i]);
		xil_printf("\n\r");

		for(int i = 0; i<8; i++)
			xil_printf("%08x", o_PMy[i]);
		xil_printf("\n\n\r");
	}




	result = compareArray(PMx_int, PMy_int, o_PMx, o_PMy);
	if(result)
		xil_printf("Encryption/Decryption success!\n\r");
	else
		xil_printf("Encryption/Decryption fail!\n\r");


	free(o_PMx);
	free(o_PMy);

	free(Px_int);	free(Px_str);
	free(Py_int);	free(Py_str);
	free(PMx_int);	free(PMx_str);
	free(PMy_int);	free(PMy_str);

	free(C1x_int);
	free(C1y_int);
	free(C2x_int);
	free(C2y_int);

	free(privKey_int);
	free(privKey_str);

   return 0;
}
