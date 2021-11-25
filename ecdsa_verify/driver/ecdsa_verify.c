// ecdsa_verify.c
// @ ZCU102
// Last Modified: 2021/11/23

#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xecdsa_verify.h"
#include "xparameters.h"
#define len 64

XEcdsa_verify ecdsa_verify;
XEcdsa_verify_Config *ecdsa_verifyPtr;


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


void verify_setup(u32* data, u32* Qx, u32* Qy, u32* r, u32* s)
{
	int status = 0;

	ecdsa_verifyPtr = XEcdsa_verify_LookupConfig(XPAR_ECDSA_VERIFY_0_DEVICE_ID);

	if (!ecdsa_verifyPtr)
	{
	 xil_printf("ERROR: Lookup of accelerator configuration failed.\n\r");
	 // return XST_FAILURE;
	}

	status = XEcdsa_verify_CfgInitialize(&ecdsa_verify, ecdsa_verifyPtr);
	if (status != XST_SUCCESS)
	{
	 xil_printf("ERROR: Could not initialize accelerator.\n\r");
	 // return XST_FAILURE;
	}


	XEcdsa_verify_Hash_v i_data;
	XEcdsa_verify_Qx_v i_Qx;
	XEcdsa_verify_Qy_v i_Qy;
	XEcdsa_verify_R_v i_r;
	XEcdsa_verify_S_v i_s;


	 i_data.word_0 = data[0];
	 i_data.word_1 = data[1];
	 i_data.word_2 = data[2];
	 i_data.word_3 = data[3];
	 i_data.word_4 = data[4];
	 i_data.word_5 = data[5];
	 i_data.word_6 = data[6];
	 i_data.word_7 = data[7];

	 i_Qx.word_0 = Qx[0];
	 i_Qx.word_1 = Qx[1];
	 i_Qx.word_2 = Qx[2];
	 i_Qx.word_3 = Qx[3];
	 i_Qx.word_4 = Qx[4];
	 i_Qx.word_5 = Qx[5];
	 i_Qx.word_6 = Qx[6];
	 i_Qx.word_7 = Qx[7];

	 i_Qy.word_0 = Qy[0];
	 i_Qy.word_1 = Qy[1];
	 i_Qy.word_2 = Qy[2];
	 i_Qy.word_3 = Qy[3];
	 i_Qy.word_4 = Qy[4];
	 i_Qy.word_5 = Qy[5];
	 i_Qy.word_6 = Qy[6];
	 i_Qy.word_7 = Qy[7];

	 i_r.word_0 = r[0];
	 i_r.word_1 = r[1];
	 i_r.word_2 = r[2];
	 i_r.word_3 = r[3];
	 i_r.word_4 = r[4];
	 i_r.word_5 = r[5];
	 i_r.word_6 = r[6];
	 i_r.word_7 = r[7];

	 i_s.word_0 = s[0];
	 i_s.word_1 = s[1];
	 i_s.word_2 = s[2];
	 i_s.word_3 = s[3];
	 i_s.word_4 = s[4];
	 i_s.word_5 = s[5];
	 i_s.word_6 = s[6];
	 i_s.word_7 = s[7];

	XEcdsa_verify_Set_hash_V(&ecdsa_verify, i_data);
	XEcdsa_verify_Set_Qx_V(&ecdsa_verify, i_Qx);
	XEcdsa_verify_Set_Qy_V(&ecdsa_verify, i_Qy);
	XEcdsa_verify_Set_r_V(&ecdsa_verify, i_r);
	XEcdsa_verify_Set_s_V(&ecdsa_verify, i_s);

}

u32 verify()
{
	int verify_finish = 0;
	u32 verify_result = 0;



	xil_printf("Start verifying \n\r");
	XEcdsa_verify_Start(&ecdsa_verify);

	while(!verify_finish)
	{
		while(XEcdsa_verify_IsDone(&ecdsa_verify))
		{
		    xil_printf("... \r\n");
			verify_result = XEcdsa_verify_Get_ifValid(&ecdsa_verify);
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


    xil_printf("\n\rStart ECDSA Verify Program \n\n\r");

    char* data_str;
    char* Qx_str;
    char* Qy_str;
    char* r_str;
    char* s_str;

	u32* data_int;
	u32* Qx_int;
	u32* Qy_int;
	u32* r_int;
	u32* s_int;

    u32 verify_result = 0;

    data_str = malloc(sizeof(char)*64);
    Qx_str = malloc(sizeof(char)*64);
    Qy_str = malloc(sizeof(char)*64);
    r_str = malloc(sizeof(char)*64);
    s_str = malloc(sizeof(char)*64);

	data_int = malloc(sizeof(int)*8);
	Qx_int = malloc(sizeof(int)*8);
	Qy_int = malloc(sizeof(int)*8);
	r_int = malloc(sizeof(int)*8);
	s_int = malloc(sizeof(int)*8);

    data_str = "4b688df40bcedbe641ddb16ff0a1842d9c67ea1c3bf63f3e0471baa664531d1a";
    Qx_str = "779dd197a5df977ed2cf6cb31d82d43328b790dc6b3b7d4437a427bd5847dfcd";
    Qy_str = "e94b724a555b6d017bb7607c3e3281daf5b1699d6ef4124975c9237b917d426f";
    r_str  = "241097efbf8b63bf145c8961dbdf10c310efbb3b2676bbc0f8b08505c9e2f795";
    s_str  = "021006b7838609339e8b415a7f9acb1b661828131aef1ecbc7955dfb01f3ca0e";




	char *dataPtr = &data_str[0];
	char *QxPtr	  = &Qx_str[0];
	char *QyPtr	  = &Qy_str[0];
	char *rPtr	  = &r_str[0];
	char *sPtr	  = &s_str[0];

	char2int_arr(dataPtr, &data_int[0]);
	char2int_arr(QxPtr, &Qx_int[0]);
	char2int_arr(QyPtr, &Qy_int[0]);
	char2int_arr(rPtr, &r_int[0]);
	char2int_arr(sPtr, &s_int[0]);


    xil_printf("data: \n\r");
    xil_printf("%s \n\n\r", data_str);

    xil_printf("signature: \n\r");
    xil_printf("%s", r_str);
    xil_printf("%s \n\n\r", s_str);

    xil_printf("public key: \n\r");
    xil_printf("%s", Qx_str);
    xil_printf("%s \n\n\r", Qy_str);


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

    free(data_str);
    free(Qx_str);
    free(Qy_str);
    free(r_str);
    free(s_str);

    return 0;
}
