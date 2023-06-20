#include <iostream>
#include <stdlib.h>

using namespace std;

uint8_t GF256_add(uint8_t a, uint8_t b, uint8_t mx){
    return a ^ b;
}

uint8_t GF256_mult_x(uint8_t a, uint8_t mx){
    if(a < 0x80) a = a << 1;
    else {
        a = a - 0x80;
        a = a << 1;
        a = a ^ mx;
    }
    return a;
}

uint8_t GF256_mult(uint8_t a, uint8_t b, uint8_t mx){
    uint8_t result = 0;
    uint8_t a_tmp = a;
    int times = 0;
    while(b){
        if((b & 0x01) != 0x00){
            a_tmp = a;
            for(int i = 0; i < times; i++) a_tmp = GF256_mult_x(a_tmp, mx);
            result = GF256_add(result, a_tmp, mx);
        }
        times++;
        b = b >> 1;
    }
    return result;
}

// GF(256) / {0} is a group under multiplication, therefore for any element a != 0 in GF(256) a^256 = 1
// --> a^255 = a^-1
uint8_t GF256_inv(uint8_t a, uint8_t mx){
    uint8_t result = a;
    for(int i = 1; i < 7; i++) result = GF256_mult(GF256_mult(result, result, mx), a, mx);
    return GF256_mult(result, result, mx);
}

void swap(uint8_t& a, uint8_t& b){
    uint8_t tmp = a;
    a = b;
    b = tmp;
}