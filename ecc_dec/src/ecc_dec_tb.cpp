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

#include "ecdsa.hpp"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#define W 256

void ecc_dec(
    ap_uint<W> C1x,
    ap_uint<W> C1y,
    ap_uint<W> C2x,
    ap_uint<W> C2y,
    ap_uint<W> privateKey,
    ap_uint<W> &PMx,
    ap_uint<W> &PMy)
          {

#pragma HLS INTERFACE s_axilite port=C1x
#pragma HLS INTERFACE s_axilite port=C1y
#pragma HLS INTERFACE s_axilite port=C2x
#pragma HLS INTERFACE s_axilite port=C2y
#pragma HLS INTERFACE s_axilite port=privateKey
#pragma HLS INTERFACE s_axilite port=PMx
#pragma HLS INTERFACE s_axilite port=PMy
#pragma HLS INTERFACE s_axilite port=return
    xf::security::ecdsaSecp256k1<256> processor;
    processor.init();
    processor.decrypt(C1x, C1y, C2x, C2y, privateKey, PMx, PMy);
}

int main() {

    ap_uint<W> privateKey = ap_uint<256>("0x90f45561b5a43da27f35706348bf86a4759b238a58a0eddb242aa264d0f02f55");

    ap_uint<W> C1x = ap_uint<256>("0x74BC99F4A27041C593B5363C60D870FFD4B0B7CEE1D7EC10904DF7D843A1A5BF");
    ap_uint<W> C1y = ap_uint<256>("0x06C8B081F3A592CA35C27B5DFD0C1807B2CF697DC9493106816E98BB1886E321");
    ap_uint<W> C2x = ap_uint<256>("0x252F9EB5FE83F91F8435051A82EC82488099916377C2192C7835CEA2ED750AAD");
    ap_uint<W> C2y = ap_uint<256>("0x94CAD6A14FE2E13843FF3E4BD11C081729D5A000D3565253616047C855094355");

    ap_uint<W> gold_PMx = ap_uint<256>("0x3031323334353637383930313233343536373839303132333435363738393031");
    ap_uint<W> gold_PMy = ap_uint<256>("0x2222222222222222222222222222222222222222222222222222222222222222");

    ap_uint<W> PMx, PMy;

    ecc_dec(C1x, C1y, C2x, C2y, privateKey, PMx, PMy);

    if (gold_PMx != PMx || gold_PMy != PMy) {
        return 1;
    } else {
        cout << hex << "PMx = " << PMx << endl;
        cout << hex << "PMy = " << PMy << endl;        
        return 0;
    }
}
