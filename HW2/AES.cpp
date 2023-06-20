#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iomanip>
#include "GF256.h"

using namespace std;

uint8_t mx = 0x1b;
uint8_t Affine[9] = {0xf1, 0xe3, 0xc7, 0x8f, 0x1f, 0x3e, 0x7c, 0xf8, 0x63};
uint8_t Mix[4][4] = {{0x02, 0x03, 0x01, 0x01}, 
                     {0x01, 0x02, 0x03, 0x01}, 
                     {0x01, 0x01, 0x02, 0x03}, 
                     {0x03, 0x01, 0x01, 0x02}};

void AES_Encrypt(uint8_t Plaintext[][4], uint8_t Ciphertext[][4], uint8_t Key[][4]);
bool count_one_even(uint8_t a);

int main(void){
    uint8_t Plaintext[4][4];
    uint8_t Ciphertext[4][4];
    uint8_t Key[4][4];
    uint8_t a, b;

    // input the Plaintext and change each byte to the exact value ex."a6" -> 10100110
    cout << "Plaintext:";
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            cin >> a >> b;
            a = (a > 0x39) ? a - 0x57 : a - 0x30;
            b = (b > 0x39) ? b - 0x57 : b - 0x30;
            Plaintext[row][col] = (a << 4) + b;
        }
    }

    // input the Key and change each byte to the exact value ex."a6" -> 10100110
    cout << "Key:";
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            cin >> a >> b;
            a = (a > 0x39) ? a - 0x57 : a - 0x30;
            b = (b > 0x39) ? b - 0x57 : b - 0x30;
            Key[row][col] = (a << 4) + b;
        }
    }

    AES_Encrypt(Plaintext, Ciphertext, Key);

    return 0;
}

void AES_Encrypt(uint8_t Plaintext[][4], uint8_t Ciphertext[][4], uint8_t Key[][4]){
    uint8_t tmp = 0;
    uint8_t RC = 0x01;
    uint8_t C_tmp[4][4];

    // Add Round Key
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            Ciphertext[row][col] = Plaintext[row][col] ^ Key[row][col];
        }
    }

    // Round 1 ~ Round 9    
    for(int round = 1; round < 10; round++){
        // Byte Substitution
        for(int col = 0; col < 4; col++) {
            for(int row = 0; row < 4; row++){
                tmp = 0x00;
                Ciphertext[row][col] = GF256_inv(Ciphertext[row][col], mx);                                 // calculate inverse in GF(256)
                for(uint8_t l = 0x00,  k = 0x01; l < 0x08; l = l + 0x01, k = k << 1){                       // Affine Transform
                    if(!count_one_even(Ciphertext[row][col] & Affine[l])) tmp = GF256_add(tmp, k, mx);  
                }
                Ciphertext[row][col] = tmp ^ Affine[8];
            }
        }

        // Shift Rows(index from 0)
        // 1st row
        tmp = Ciphertext[1][0];
        for(int col = 0; col < 3; col++){
            Ciphertext[1][col] = Ciphertext[1][col + 1];
        }
        Ciphertext[1][3] = tmp;
        // 2nd row
        swap(Ciphertext[2][0], Ciphertext[2][2]);
        swap(Ciphertext[2][1], Ciphertext[2][3]);
        //3rd row
        tmp = Ciphertext[3][3];
        for(int col = 3; col > 0; col--){
            Ciphertext[3][col] = Ciphertext[3][col - 1];
        }
        Ciphertext[3][0] = tmp;

        // Mix Columns
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                C_tmp[row][col] = 0x00;
                for(int index = 0; index < 4; index++){
                    C_tmp[row][col] = GF256_add(C_tmp[row][col], GF256_mult(Mix[row][index], Ciphertext[index][col], mx), mx); 
                }
            }
        }
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                Ciphertext[row][col] = C_tmp[row][col];
            }
        }

        // Key Expansion
        // |    |    |    |    |          |    |    |    |    |
        // | W0 | W1 | W2 | W3 | ------>  | W4 | W5 | W6 | W7 |
        // |    |    |    |    |          |    |    |    |    |
        // Calculate W3*
        for(int col = 0; col < 4; col++) {
            tmp = 0x00;
            C_tmp[0][col] = GF256_inv(Key[(col + 1) % 4][3], mx);                           // shift W3 and calculate inverse
            for(uint8_t l = 0x00,  k = 0x01; l < 0x08; l = l + 0x01, k = k << 1){           // Affine Transform
                if(!count_one_even(C_tmp[0][col] & Affine[l])) tmp = GF256_add(tmp, k, mx);  
            }
            C_tmp[0][col] = tmp ^ Affine[8];
            if(col == 0) C_tmp[0][col] = GF256_add(C_tmp[0][col], RC, mx);                  // Add RC[i]
        }
        RC = GF256_mult_x(RC, mx);                                                          // Calculate RC[i + 1]
        for(int col = 0; col < 4; col++){                                                   // W4 = W4 + W3*, W5 = W5 + W4, W6 = W6 + W5, W7 = W7 + W6
            for(int row = 0; row < 4; row++){
                if(col == 0) Key[row][col] = GF256_add(Key[row][col], C_tmp[0][row], mx);
                else Key[row][col] = GF256_add(Key[row][col], Key[row][col - 1], mx);
            }
        }

        // Add Round Key
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                Ciphertext[row][col] = Ciphertext[row][col] ^ Key[row][col];
            }
        }

        // Output the result
        cout << "Round " << round << ":" << endl;
        for(int col = 0; col < 4; col++){
            for(int row = 0; row < 4; row++){
                cout << setfill('0') << setw(2) << hex << (int)Ciphertext[row][col] << dec << " ";
            }
        }
        cout << endl;
    }

    // Round 10
    // Byte Substitution
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 4; row++){
            tmp = 0x00;
            Ciphertext[row][col] = GF256_inv(Ciphertext[row][col], mx);
            for(uint8_t l = 0x00,  k = 0x01; l < 0x08; l = l + 0x01, k = k << 1){
                if(!count_one_even(Ciphertext[row][col] & Affine[l])) tmp = GF256_add(tmp, k, mx);  
            }
            Ciphertext[row][col] = tmp ^ Affine[8];
        }
    }
    // Shift Rows(index from 0)
    // 1st row
    tmp = Ciphertext[1][0];
    for(int col = 0; col < 3; col++){
        Ciphertext[1][col] = Ciphertext[1][col + 1];
    }
    Ciphertext[1][3] = tmp;
    // 2nd row
    swap(Ciphertext[2][0], Ciphertext[2][2]);
    swap(Ciphertext[2][1], Ciphertext[2][3]);
    //3rd row
    tmp = Ciphertext[3][3];
    for(int col = 3; col > 0; col--){
        Ciphertext[3][col] = Ciphertext[3][col - 1];
    }
    Ciphertext[3][0] = tmp;

    // Key Expansion
    for(int col = 0; col < 4; col++) {
        tmp = 0x00;
        C_tmp[0][col] = GF256_inv(Key[(col + 1) % 4][3], mx);
        for(uint8_t l = 0x00,  k = 0x01; l < 0x08; l = l + 0x01, k = k << 1){
            if(!count_one_even(C_tmp[0][col] & Affine[l])) tmp = GF256_add(tmp, k, mx);  
        }
        C_tmp[0][col] = tmp ^ Affine[8];
        if(col == 0) C_tmp[0][col] = GF256_add(C_tmp[0][col], RC, mx);
    }
    RC = GF256_mult_x(RC, mx);
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(col == 0) Key[row][col] = GF256_add(Key[row][col], C_tmp[0][row], mx);
            else Key[row][col] = GF256_add(Key[row][col], Key[row][col - 1], mx);
        }
    }

    // Add Round Key
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            Ciphertext[row][col] = Ciphertext[row][col] ^ Key[row][col];
        }
    }
    cout << "Round " << 10 << ":" << endl;
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            cout << setfill('0') << setw(2) << hex << (int)Ciphertext[row][col] << dec << " ";
        }
    }
    cout << endl;
}

bool count_one_even(uint8_t a){
    bool result = true;
    for(uint8_t k = 0x01,  t = 0x00; t < 0x08; t = t + 0x01, k = k << 1){
        if((a & k) != 0x00) result = !result;
    }
    return result;
}