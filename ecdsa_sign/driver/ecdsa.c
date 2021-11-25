// ecdsa.c
// @ ZCU102
// Last Modified: 2021/11/25

#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xecdsa.h"
#include "xparameters.h"
#define len 64

XEcdsa ecdsa;
XEcdsa_Config *ecdsaPtr;


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

int ecdsa_init()
{
	int status = 0;

	ecdsaPtr = XEcdsa_LookupConfig(XPAR_ECDSA_0_DEVICE_ID);

	if (!ecdsaPtr)
	{
	 xil_printf("ERROR: Lookup of accelerator configuration failed.\n\r");
	  return XST_FAILURE;
	}

	status = XEcdsa_CfgInitialize(&ecdsa, ecdsaPtr);
	if (status != XST_SUCCESS)
	{
	 xil_printf("ERROR: Could not initialize accelerator.\n\r");
	  return XST_FAILURE;
	}
	return XST_SUCCESS;
}


void verify_setup(u32* data, u32* Qx, u32* Qy, u32* r, u32* s)
{

	XEcdsa_I_hash_v i_data;
	XEcdsa_I_qx_v i_Qx;
	XEcdsa_I_qy_v i_Qy;
	XEcdsa_I_r_v i_r;
	XEcdsa_I_s_v i_s;

	 i_data.word_0 = data[0];
	 i_data.word_1 = data[1];
	 i_data.word_2 = data[2];
	 i_data.word_3 = data[3];
	 i_data.word_4 = data[4];
	 i_data.word_5 = data[5];
	 i_data.word_6 = data[6];
	 i_data.word_7 = data[7];

	 i_Qx.word_0 = Qx[7];
	 i_Qx.word_1 = Qx[6];
	 i_Qx.word_2 = Qx[5];
	 i_Qx.word_3 = Qx[4];
	 i_Qx.word_4 = Qx[3];
	 i_Qx.word_5 = Qx[2];
	 i_Qx.word_6 = Qx[1];
	 i_Qx.word_7 = Qx[0];

	 i_Qy.word_0 = Qy[7];
	 i_Qy.word_1 = Qy[6];
	 i_Qy.word_2 = Qy[5];
	 i_Qy.word_3 = Qy[4];
	 i_Qy.word_4 = Qy[3];
	 i_Qy.word_5 = Qy[2];
	 i_Qy.word_6 = Qy[1];
	 i_Qy.word_7 = Qy[0];

	 i_r.word_0 = r[7];
	 i_r.word_1 = r[6];
	 i_r.word_2 = r[5];
	 i_r.word_3 = r[4];
	 i_r.word_4 = r[3];
	 i_r.word_5 = r[2];
	 i_r.word_6 = r[1];
	 i_r.word_7 = r[0];

	 i_s.word_0 = s[7];
	 i_s.word_1 = s[6];
	 i_s.word_2 = s[5];
	 i_s.word_3 = s[4];
	 i_s.word_4 = s[3];
	 i_s.word_5 = s[2];
	 i_s.word_6 = s[1];
	 i_s.word_7 = s[0];



	XEcdsa_Set_i_hash_V(&ecdsa, i_data);

	XEcdsa_Set_i_Qx_V(&ecdsa, i_Qx);
	XEcdsa_Set_i_Qy_V(&ecdsa, i_Qy);
	XEcdsa_Set_i_r_V(&ecdsa, i_r);
	XEcdsa_Set_i_s_V(&ecdsa, i_s);
	XEcdsa_Set_i_mode(&ecdsa, 0);	//1 for sign, 0 for verify

}


void sign_setup(u32* data, u32* privKey)
{


	XEcdsa_I_hash_v    i_data;
	XEcdsa_I_k_v	   i_k;
	XEcdsa_I_privkey_v i_privKey;

	char k_str[64] = "49a0d7b786ec9cde0d0721d72804befd06571c974b191efb42ecf322ba9ddd9a";
	char *kPtr = &k_str[0];
	u32 k[8];

	char2int_arr(kPtr, &k[0]);

	 i_data.word_0 = data[0];
	 i_data.word_1 = data[1];
	 i_data.word_2 = data[2];
	 i_data.word_3 = data[3];
	 i_data.word_4 = data[4];
	 i_data.word_5 = data[5];
	 i_data.word_6 = data[6];
	 i_data.word_7 = data[7];

	 i_k.word_0 = k[0];
	 i_k.word_1 = k[1];
	 i_k.word_2 = k[2];
	 i_k.word_3 = k[3];
	 i_k.word_4 = k[4];
	 i_k.word_5 = k[5];
	 i_k.word_6 = k[6];
	 i_k.word_7 = k[7];

	 i_privKey.word_0 = privKey[0];
	 i_privKey.word_1 = privKey[1];
	 i_privKey.word_2 = privKey[2];
	 i_privKey.word_3 = privKey[3];
	 i_privKey.word_4 = privKey[4];
	 i_privKey.word_5 = privKey[5];
	 i_privKey.word_6 = privKey[6];
	 i_privKey.word_7 = privKey[7];


	 //xil_printf("Start Setting Input Value \n\r");


	 XEcdsa_Set_i_hash_V(&ecdsa, i_data);
	 XEcdsa_Set_i_k_V(&ecdsa, i_k);
	 XEcdsa_Set_i_privKey_V(&ecdsa, i_privKey);
	 XEcdsa_Set_i_mode(&ecdsa, 1);		//1 for sign, 0 for verify

}


void sign(u32* Qx, u32* Qy, u32* r, u32* s)
{
	 XEcdsa_O_qx_v o_Qx;
	 XEcdsa_O_qy_v o_Qy;
	 XEcdsa_O_r_v o_R;
	 XEcdsa_O_s_v o_S;

	 int sign_finish;

	 xil_printf("Start Signing \n\r");
	 XEcdsa_Start(&ecdsa);

	while(!sign_finish)
	{
		 while(XEcdsa_IsDone(&ecdsa))
		 {
			 xil_printf("... \r\n");
			 o_Qx = XEcdsa_Get_o_Qx_V(&ecdsa);
			 o_Qy = XEcdsa_Get_o_Qy_V(&ecdsa);
			 o_R = XEcdsa_Get_o_r_V(&ecdsa);
			 o_S = XEcdsa_Get_o_s_V(&ecdsa);

			 Qx[0] = o_Qx.word_7;
			 Qx[1] = o_Qx.word_6;
			 Qx[2] = o_Qx.word_5;
			 Qx[3] = o_Qx.word_4;
			 Qx[4] = o_Qx.word_3;
			 Qx[5] = o_Qx.word_2;
			 Qx[6] = o_Qx.word_1;
			 Qx[7] = o_Qx.word_0;

			 Qy[0] = o_Qy.word_7;
			 Qy[1] = o_Qy.word_6;
			 Qy[2] = o_Qy.word_5;
			 Qy[3] = o_Qy.word_4;
			 Qy[4] = o_Qy.word_3;
			 Qy[5] = o_Qy.word_2;
			 Qy[6] = o_Qy.word_1;
			 Qy[7] = o_Qy.word_0;

			 r[0] = o_R.word_7;
			 r[1] = o_R.word_6;
			 r[2] = o_R.word_5;
			 r[3] = o_R.word_4;
			 r[4] = o_R.word_3;
			 r[5] = o_R.word_2;
			 r[6] = o_R.word_1;
			 r[7] = o_R.word_0;

			 s[0] = o_S.word_7;
			 s[1] = o_S.word_6;
			 s[2] = o_S.word_5;
			 s[3] = o_S.word_4;
			 s[4] = o_S.word_3;
			 s[5] = o_S.word_2;
			 s[6] = o_S.word_1;
			 s[7] = o_S.word_0;

			 sign_finish = 1;
		 }

		 if(sign_finish){
			 xil_printf("Finish signing \n\n\r");
			// xil_printf("kValid: %d\n\r", kValid);
		 }
	}

}


u32 verify()
{
	int verify_finish = 0;
	u32 verify_result = 0;

	xil_printf("Start verifying \n\r");
	XEcdsa_Start(&ecdsa);

	while(!verify_finish)
	{
		while(XEcdsa_IsDone(&ecdsa))
		{
		    xil_printf("... \r\n");
			verify_result = XEcdsa_Get_o_Valid(&ecdsa);
			verify_finish = 1;
		}
		if(verify_finish)
		{
			xil_printf("Finish verifying \r\n\n");
		}
	}
	return verify_result;
}


int main()
{

	xil_printf("Start ECDSA Program \n\n\r");

	int status, verify_result;

	char* data_str;
	char* privKey_str;

	u32* data_int;
	u32* privKey_int;

	u32* Qx_int;
	u32* Qy_int;
	u32* r_int;
	u32* s_int;


	data_str = malloc(sizeof(char)*64);
	privKey_str = malloc(sizeof(char)*64);

	data_int = malloc(sizeof(int)*8);
	privKey_int = malloc(sizeof(int)*8);

	Qx_int = malloc(sizeof(int)*8);
	Qy_int = malloc(sizeof(int)*8);
	r_int = malloc(sizeof(int)*8);
	s_int = malloc(sizeof(int)*8);




	for(int i = 0; i<8; i++)
	{
		Qx_int[i] = 0;
		Qy_int[i] = 0;
		r_int[i] = 0;
		s_int[i] = 0;

		data_int[i] = 0;
		privKey_int[i] = 0;
	}



	data_str 	= "4b688df40bcedbe641ddb16ff0a1842d9c67ea1c3bf63f3e0471baa664531d1a";	// \0
	privKey_str = "ebb2c082fd7727890a28ac82f6bdf97bad8de9f5d7c9028692de1a255cad3e0f";

	////////// Initialization ///////////

	status = ecdsa_init();
	if(status == XST_SUCCESS)
		xil_printf("Successfully initialize ECDSA \r\n\n");
	else
		xil_printf("Fail to initialize ECDSA \r\n\n");

	////////// Sign ///////////

	char *dataPtr 	 = &data_str[0];
	char *privKeyPtr = &privKey_str[0];


	char2int_arr(dataPtr, &data_int[0]);
	char2int_arr(privKeyPtr, &privKey_int[0]);


	xil_printf("data: \n\r");
	xil_printf("%s \n\n\r", data_str);

	xil_printf("private key: \n\r");
	xil_printf("%s \n\n\r", privKey_str);

	sign_setup(data_int, privKey_int);
	sign(Qx_int, Qy_int, r_int, s_int);


	xil_printf("public key: \n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", Qx_int[i]);
	xil_printf("\n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", Qy_int[i]);

	xil_printf("\n\n\r");

	xil_printf("signature: \n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", r_int[i]);
	xil_printf("\n\r");
	for(int i = 0; i<8; i++)
		xil_printf("%08x", s_int[i]);
	xil_printf("\n\n\r");



	////////// Verify //////////


	verify_setup(data_int, Qx_int, Qy_int, r_int, s_int);

	verify_result = verify();


    if(verify_result==1)
    	xil_printf("verify success! \n\r");
    else
    	xil_printf("verify fail! \n\r");

    free(data_int);
    free(Qx_int);
    free(Qy_int);
    free(r_int);
    free(s_int);
    free(privKey_int);

    free(data_str);
    free(privKey_str);



	return 0;
}

