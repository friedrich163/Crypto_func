// ecc_enc.c
// @ ZCU102
// Last Modified: 2021/11/26

#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xecc_enc.h"
#include "xparameters.h"
#define len 64


XEcc_enc ecc_enc;
XEcc_enc_Config *ecc_encPtr;

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



int ecc_encrypt_init()
{
	int status = 0;

	ecc_encPtr = XEcc_enc_LookupConfig(XPAR_ECC_ENC_0_DEVICE_ID);

	if (!ecc_encPtr)
	{
	 xil_printf("ERROR: Lookup of accelerator configuration failed.\n\r");
	  return XST_FAILURE;
	}

	status = XEcc_enc_CfgInitialize(&ecc_enc, ecc_encPtr);
	if (status != XST_SUCCESS)
	{
	 xil_printf("ERROR: Could not initialize accelerator.\n\r");
	  return XST_FAILURE;
	}
	return XST_SUCCESS;
}



void ecc_encrypt_setup( u32* Px, u32* Py, u32* PMx, u32* PMy )
{
	XEcc_enc_Px_v i_Px;
	XEcc_enc_Py_v i_Py;
	XEcc_enc_Pmx_v i_PMx;
	XEcc_enc_Pmy_v i_PMy;
	XEcc_enc_Randomkey_v i_randomKey;


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

	XEcc_enc_Set_Px_V(&ecc_enc, i_Px);
	XEcc_enc_Set_Py_V(&ecc_enc, i_Py);

	XEcc_enc_Set_PMx_V(&ecc_enc, i_PMx);
	XEcc_enc_Set_PMy_V(&ecc_enc, i_PMy);
	XEcc_enc_Set_randomKey_V(&ecc_enc, i_randomKey);

}

void ecc_encrypt(u32* C1x, u32* C1y, u32* C2x, u32* C2y)
{
	XEcc_enc_C1x_v o_C1x;
	XEcc_enc_C1y_v o_C1y;
	XEcc_enc_C2x_v o_C2x;
	XEcc_enc_C2y_v o_C2y;

	int enc_finish;

	xil_printf("Start Encrypting \n\r");
	XEcc_enc_Start(&ecc_enc);

	while(!enc_finish)
	{
		while(XEcc_enc_IsDone(&ecc_enc))
		{
			 xil_printf("... \r\n");
			 o_C1x = XEcc_enc_Get_C1x_V(&ecc_enc);
			 o_C2x = XEcc_enc_Get_C2x_V(&ecc_enc);
			 o_C1y = XEcc_enc_Get_C1y_V(&ecc_enc);
			 o_C2y = XEcc_enc_Get_C2y_V(&ecc_enc);

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

}


int main()
{

    xil_printf("Start ECC Encrypt Project\n\r");

    int status;

    char* Px_str;
    char* Py_str;
    char* PMx_str;
    char* PMy_str;

    u32* Px_int;
    u32* Py_int;
    u32* PMx_int;
    u32* PMy_int;

    u32* C1x;
    u32* C1y;
    u32* C2x;
    u32* C2y;

    Px_str = malloc(sizeof(char)*64);
    Py_str = malloc(sizeof(char)*64);
    PMx_str = malloc(sizeof(char)*64);
    PMy_str = malloc(sizeof(char)*64);

    Px_int = malloc(sizeof(int)*8);
    Py_int = malloc(sizeof(int)*8);
    PMx_int = malloc(sizeof(int)*8);
    PMx_int = malloc(sizeof(int)*8);

    C1x = malloc(sizeof(int)*8);
    C1y = malloc(sizeof(int)*8);
    C2x = malloc(sizeof(int)*8);
    C2y = malloc(sizeof(int)*8);


    for(int i = 0; i<8; i++)
    {
    	Px_int[i] = 0;	PMx_int[i] = 0;
    	Py_int[i] = 0;	PMy_int[i] = 0;

    	C1x[i] = 0; C1y[i] = 0;
    	C2x[i] = 0; C2y[i] = 0;
    }

    Px_str = "f256c6e6c80b213f2aa0b01744235d515c594435be651b15883a10dd472fa646";
    Py_str = "ce62eaf3670dc5cb9100a0ca2a55b2c147c1e9a38ce428878e7d46e1fb714a99";
	PMx_str = "4041424344454647484940414243444546474849404142434445464748494041";
	PMy_str = "3031323334353637383930313233343536373839303132333435363738393031";

	///////////// Initialization /////////////

	status = ecc_encrypt_init();
	if(status == XST_SUCCESS)
		xil_printf("Successfully initialize ECC Encrypt \r\n\n");
	else
		xil_printf("Fail to initialize ECC Encrypt \r\n\n");

	///////////// Encrypt /////////////

	char *PxPtr = &Px_str[0];
	char *PyPtr = &Py_str[0];
	char *PMxPtr = &PMx_str[0];
	char *PMyPtr = &PMy_str[0];

	char2int_arr(PxPtr, &Px_int[0]);
	char2int_arr(PyPtr, &Py_int[0]);
	char2int_arr(PMxPtr, &PMx_int[0]);
	char2int_arr(PMyPtr, &PMy_int[0]);

	xil_printf("data: \n\r");
	xil_printf("%s \n\r", PMx_str);
	xil_printf("%s \n\n\r", PMy_str);

	xil_printf("public key: \n\r");
	xil_printf("%s \n\r", Px_str);
	xil_printf("%s \n\n\r", Py_str);

	ecc_encrypt_setup(Px_int, Py_int, PMx_int, PMy_int);
	ecc_encrypt(C1x, C1y, C2x, C2y);

	xil_printf("Cipher Text: \n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", C1x[i]);
	xil_printf("\n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", C1y[i]);
	xil_printf("\n\n\r");

	for(int i = 0; i<8; i++)
		xil_printf("%08x", C2x[i]);
	xil_printf("\n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", C2y[i]);
	xil_printf("\n\n\r");

	free(C1x);
	free(C1y);
	free(C2x);
	free(C2y);

	free(Px_int);	free(Px_str);
	free(Py_int);	free(Py_str);
	free(PMx_int);	free(PMx_str);
	free(PMy_int);	free(PMy_str);

    return 0;
}
