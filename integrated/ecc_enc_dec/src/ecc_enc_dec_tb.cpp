/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


// #include "ecc.hpp"
#include "ecdsa.hpp"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#ifndef __SYNTHESIS__
#include <iostream>
using namespace std;
#endif

#define W 256

void ecc_enc_dec(
          bool i_mode,
          ap_uint<W> i_Px_C1x,
          ap_uint<W> i_Py_C1y,
          ap_uint<W> i_PMx_C2x,
          ap_uint<W> i_PMy_C2y,
          ap_uint<W> i_randomKey_privKey,

          ap_uint<W>& o_C1x_PMx,
          ap_uint<W>& o_C1y_PMy,
          ap_uint<W>& o_C2x,
          ap_uint<W>& o_C2y
          ) 
          {
    xf::security::ecdsaSecp256k1<256> processor;
    processor.init();              
#pragma HLS INTERFACE s_axilite port=i_mode
#pragma HLS INTERFACE s_axilite port=i_Px_C1x
#pragma HLS INTERFACE s_axilite port=i_Py_C1y
#pragma HLS INTERFACE s_axilite port=i_PMx_C2x
#pragma HLS INTERFACE s_axilite port=i_PMy_C2y
#pragma HLS INTERFACE s_axilite port=i_randomKey_privKey
#pragma HLS INTERFACE s_axilite port=o_C1x_PMx
#pragma HLS INTERFACE s_axilite port=o_C1y_PMy
#pragma HLS INTERFACE s_axilite port=o_C2x
#pragma HLS INTERFACE s_axilite port=o_C2y

#pragma HLS INTERFACE s_axilite port=return

    if(i_mode)
        {        
            // processor.encrypt(Px, Py, randomKey, PMx, PMy, C1x, C1y, C2x, C2y);
            processor.encrypt(i_Px_C1x, i_Py_C1y, i_randomKey_privKey, i_PMx_C2x, i_PMy_C2y, o_C1x_PMx, o_C1y_PMy, o_C2x, o_C2y);

        }
    else
        {
            // processor.decrypt(C1x, C1y, C2x, C2y, privateKey, PMx, PMy)
            processor.decrypt(i_Px_C1x, i_Py_C1y, i_PMx_C2x, i_PMy_C2y, i_randomKey_privKey, o_C1x_PMx, o_C1y_PMy);

            o_C2x = 0;
            o_C2y = 0;

        }
}
#ifndef __SYNTHESIS__
int main() {

    bool mode;
    ap_uint<W> pubKey_x = ap_uint<256>("0xf256c6e6c80b213f2aa0b01744235d515c594435be651b15883a10dd472fa646");
    ap_uint<W> pubKey_y = ap_uint<256>("0xce62eaf3670dc5cb9100a0ca2a55b2c147c1e9a38ce428878e7d46e1fb714a99");

    // ap_uint<W> PMx =  ap_uint<256>("0x1111111111111111111111111111111111111111111111111111111111111111");
    ap_uint<W> PMx = ap_uint<256>("0x4041424344454647484940414243444546474849404142434445464748494041");
    ap_uint<W> PMy = ap_uint<256>("0x3031323334353637383930313233343536373839303132333435363738393031");
    ap_uint<W> randomKey = ap_uint<256>("0x2222222222222222222222222222222222222222222222211111111111111111");
    ap_uint<W> privKey = ap_uint<256>("0x90f45561b5a43da27f35706348bf86a4759b238a58a0eddb242aa264d0f02f55");



    ap_uint<W> C1x, C1y, C2x, C2y, o_PMx, o_PMy, o_x, o_y;

    cout<<"Encryption..."<<endl;
    mode = 1;
    ecc_enc_dec(mode, pubKey_x, pubKey_y, PMx, PMy, randomKey, C1x, C1y, C2x, C2y);
    cout << hex <<"plaintext x: " << PMx << endl;
    cout << hex <<"plaintext y: " << PMy << endl;    


    cout<<"Decryption..."<<endl;
    mode = 0;
    ecc_enc_dec(mode, C1x, C1y, C2x, C2y, privKey, o_PMx, o_PMy, o_x, o_y);
    cout << hex <<"Output plaintext x: " << o_PMx << endl;
    cout << hex <<"Output plaintext y: " << o_PMy << endl;


    if( PMx != o_PMx || PMy != o_PMy )
        return 1;
    else
        return 0;
}
#endif