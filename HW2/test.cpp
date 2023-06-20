#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iomanip>
#include "GF256.h"

using namespace std;

uint8_t mx = 0x1b;
uint8_t Affine[9] = {0xf1, 0xe3, 0xc7, 0x8f, 0x1f, 0x3e, 0x7c, 0xf8, 0x63};

bool count_one_even(uint8_t a){
    bool result = true;
    for(uint8_t k = 0x01,  t = 0x00; t < 0x08; t = t + 0x01, k = k << 1){
        if((a & k) != 0x00) result = !result;
    }
    return result;
}

int main(void){
    uint8_t tmp = GF256_mult(0xfa, 0x03, mx);
    cout << setfill('0') << setw(2) << hex << (int)tmp << dec << " ";

    return 0;
}