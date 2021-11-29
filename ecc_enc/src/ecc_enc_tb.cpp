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
#include <iostream>
using namespace std;
#define W 256

void ecc_enc(
          ap_uint<W> Px,
          ap_uint<W> Py,
          ap_uint<W> randomKey,
          ap_uint<W> PMx,
          ap_uint<W> PMy,
          ap_uint<W>& C1x,
          ap_uint<W>& C1y,
          ap_uint<W>& C2x,
          ap_uint<W>& C2y) 
          {

#pragma HLS INTERFACE s_axilite port=Px
#pragma HLS INTERFACE s_axilite port=Py
#pragma HLS INTERFACE s_axilite port=randomKey
#pragma HLS INTERFACE s_axilite port=PMx
#pragma HLS INTERFACE s_axilite port=PMy
#pragma HLS INTERFACE s_axilite port=C1x
#pragma HLS INTERFACE s_axilite port=C1y
#pragma HLS INTERFACE s_axilite port=C2x
#pragma HLS INTERFACE s_axilite port=C2y
#pragma HLS INTERFACE s_axilite port=return

    xf::security::ecdsaSecp256k1<256> processor;
    processor.init();
    processor.encrypt(Px, Py, randomKey, PMx, PMy, C1x, C1y, C2x, C2y);
}

int main() {
    ap_uint<W> Px = ap_uint<256>("0xf256c6e6c80b213f2aa0b01744235d515c594435be651b15883a10dd472fa646");
    ap_uint<W> Py = ap_uint<256>("0xce62eaf3670dc5cb9100a0ca2a55b2c147c1e9a38ce428878e7d46e1fb714a99");

    // ap_uint<W> PMx =  ap_uint<256>("0x1111111111111111111111111111111111111111111111111111111111111111");
    ap_uint<W> PMx = ap_uint<256>("0x3031323334353637383930313233343536373839303132333435363738393031");
    ap_uint<W> PMy = ap_uint<256>("0x2222222222222222222222222222222222222222222222222222222222222222");
    ap_uint<W> randomKey = ap_uint<256>("0x2222222222222222222222222222222222222222222222211111111111111111");

    ap_uint<256> gold_c1x = ap_uint<256>("0x74bc99f4a27041c593b5363c60d870ffd4b0b7cee1d7ec10904df7d843a1a5bf");
    ap_uint<256> gold_c1y = ap_uint<256>("0x06c8b081f3a592ca35c27b5dfd0c1807b2cf697dc9493106816e98bb1886e321");
    ap_uint<256> gold_c2x = ap_uint<256>("0x252f9eb5fe83f91f8435051a82ec82488099916377c2192c7835cea2ed750aad");
    ap_uint<256> gold_c2y = ap_uint<256>("0x94cad6a14fe2e13843ff3e4bd11c081729d5a000d3565253616047c855094355");    


    ap_uint<W> C1x, C1y, C2x, C2y;

    ecc_enc(Px, Py, randomKey, PMx, PMy, C1x, C1y, C2x, C2y);

    cout << "Px = " << hex << Px << endl;
    cout << "Py = " << hex << Py << endl;
    cout << "PMx = " << hex << PMx << endl;
    cout << "PMy = " << hex << PMy << endl;
    cout << "C1x = " << hex << C1x << endl;
    cout << "C1y = " << hex << C1y << endl;
    cout << "C2x = " << hex << C2x << endl;
    cout << "C2y = " << hex << C2y << endl;

    if( C1x != gold_c1x || C1y != gold_c1y || C2x != gold_c2x || C2y != gold_c2y )
        return 1;
    else
        return 0;
}
