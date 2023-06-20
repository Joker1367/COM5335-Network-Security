#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <string>
#include "BigNumber.h"

BigNumber Find_SQR(BigNumber a, BigNumber P);
BigNumber fastpower(BigNumber a, BigNumber power, BigNumber P);
bool Miller_Rabin(BigNumber Prime, int iteration);
void EEA(BigNumber N, BigNumber a, BigNumber b, BigNumber& s0, BigNumber& s1, BigNumber& t0, BigNumber& t1);

BigNumber prime_table[168] = {2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31, 
                        37,  41,  43,  47,  53,  59,  61,  67,  71,  73,  79, 
                        83,  89,  97,  101, 103, 107, 109, 113, 127, 131, 137, 
                        139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 
                        197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 
                        263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 
                        331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 
                        397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 
                        461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 
                        541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 
                        607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 
                        673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 
                        751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 
                        827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 
                        907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 
                        983, 991, 997};

int main(void){
// <Miller-Rabin>
    srand(30);
    cout << "<Miller-Rabin>\n";
    BigNumber Prime(1, 64);
    int iteration = 5;                                              // number of iteration for miller rabin
    bool isprime = false;                                           
    while(!isprime){
        isprime = true;
        Prime.data[63] = rand() % (MAX - 1) + 1;
        for(int i = 0; i < 63; i++) Prime.data[i] = rand() % MAX;   // generate random 256-bit number
        for(BigNumber p: prime_table){                              // test for small prime
            if(Prime % p == 0) {
                isprime = false;
                break;
            }
        }
        //cout << "start Miller Rabin\n";
        if(isprime) isprime = Miller_Rabin(Prime, iteration);       // start miller-rabin
    }
    Prime.Print();                                                  // print result
    cout << endl;
// <Rabin Encryption>
    string s;
    int len;
    vector<int> data(MAX_Bit, 0);
    cout << "<Rabin Encryption>\n";
    // Input P
        cout << "p = ";
        getline(cin, s);
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        len = s.length();
        for(int i = 0; i < len; i++){
            if(s[i] >= 97) data[len - 1 - i] = s[i] - 97 + 10;
            else data[len - 1 - i] = s[i] - 48;
        }
        BigNumber P(1, len, data);
        for(int i = 0; i < len; i++) data[i] = 0;
        //if(Miller_Rabin(P, 5)) cout << "YES\n";
    // Input Q
        cout << "q = ";
        getline(cin, s);
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        len = s.length();
        for(int i = 0; i < len; i++){
            if(s[i] >= 97) data[len - 1 - i] = s[i] - 97 + 10;
            else data[len - 1 - i] = s[i] - 48;
        }
        BigNumber Q(1, len, data);
        for(int i = 0; i < len; i++) data[i] = 0;
        //if(Miller_Rabin(Q, 5)) cout << "YES\n";
    // Print N
        BigNumber N;
        cout << "n = pq = ";
        N = P * Q;
        N.Print();
        cout << endl;
    // Input Plaintext
        cout << "Plaintext: ";
        getline(cin, s);
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        s = s + s.substr(52, 4);
        len = s.length();
        for(int i = 0; i < len; i++){
            if(s[i] >= 97) data[len - 1 - i] = s[i] - 97 + 10;
            else data[len - 1 - i] = s[i] - 48;
        }
        BigNumber Plaintext(1, len, data);
        for(int i = 0; i < len; i++) data[i] = 0;
    // Output Ciphertext
        cout << "Ciphertext = ";
        BigNumber Ciphertext;
        Ciphertext = (Plaintext * Plaintext) % N;
        Ciphertext.Print();
        cout << endl;
// <Rabin Decryption>
    cout << "<Rabin Decryption>\n";
    // Input Ciphertext
        cout << "Ciphertext = ";
        getline(cin, s);
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        len = s.length();
        for(int i = 0; i < len; i++){
            if(s[i] >= 97) data[len - 1 - i] = s[i] - 97 + 10;
            else data[len - 1 - i] = s[i] - 48;
        }
        Ciphertext = BigNumber(1, len, data);
        for(int i = 0; i < len; i++) data[i] = 0;
    // Input P
        cout << "Private Key:\n";
        cout << "p = ";
        getline(cin, s);
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        len = s.length();
        for(int i = 0; i < len; i++){
            if(s[i] >= 97) data[len - 1 - i] = s[i] - 97 + 10;
            else data[len - 1 - i] = s[i] - 48;
        }
        P = BigNumber(1, len, data);
        for(int i = 0; i < len; i++) data[i] = 0;
        //if(Miller_Rabin(P, 5)) cout << "YES\n";
    // Input Q
        cout << "q = ";
        getline(cin, s);
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        len = s.length();
        for(int i = 0; i < len; i++){
            if(s[i] >= 97) data[len - 1 - i] = s[i] - 97 + 10;
            else data[len - 1 - i] = s[i] - 48;
        }
        Q = BigNumber(1, len, data);
        for(int i = 0; i < len; i++) data[i] = 0;
        //if(Miller_Rabin(Q, 5)) cout << "YES\n";
    // Decryption
        N = P * Q;                                                  // N = P * Q
        Ciphertext = Ciphertext % N;
        BigNumber SQR_P, SQR_Q;
        SQR_P = Find_SQR(Ciphertext, P);                            // Find SQR_P s.t SQR_P^2 = Ciphertext (mod P)
        SQR_Q = Find_SQR(Ciphertext, Q);                            // Find SQR_Q s.t SQR_Q^2 = Ciphertext (mod Q)
        BigNumber s0, s1, t0, t1;
        s0 = 1; t1 = 1;
        EEA(N, P, Q, s0, s1, t0, t1);                               // use EEA to find sp + tq = 1 (mod N)
        BigNumber Result1, Result2, Result3, Result4;
        Result1 = (SQR_P * t1 * Q + SQR_Q * s1 * P) % N;            // x = SQR_P * t1 * Q + SQR_Q * s1 * P (mod N)
        Result2 = N - Result1;                                      // -x
        Result3 = SQR_P * t1 * Q - SQR_Q * s1 * P;                  // y = SQR_P * t1 * Q - SQR_Q * s1 * P (mod N)
        if(Result3.sign == 1) Result3 = Result3 % N;                
        else{
            Result3.sign = 1;
            Result3 = Result3 % N;
            Result3 = N - Result3;
        }
        Result4 = N - Result3;                                      // -y
        Result1.num_of_bits = 60;                                   // expand to 60 bit
        Result2.num_of_bits = 60;
        Result3.num_of_bits = 60;
        Result4.num_of_bits = 60;
        /*Result1.Print();
        Result2.Print();
        Result3.Print();
        Result4.Print();*/

        // Check with repitition padding
        cout << "Plaintext = ";
        if(Result1.num_of_bits == 60 && Result1.data[7] == Result1.data[3] &&
           Result1.data[6] == Result1.data[2] && Result1.data[5] == Result1.data[1] && 
           Result1.data[4] == Result1.data[0]) {
                for(int i = 0; i < 60; i++) Result1.data[i] = Result1.data[i + 4];
                Result1.num_of_bits -= 4;
                Result1.Print();
        }
        else if(Result2.num_of_bits == 60 && Result2.data[7] == Result2.data[3] &&
                Result2.data[6] == Result2.data[2] && Result2.data[5] == Result2.data[1] && 
                Result2.data[4] == Result2.data[0]) {
                    for(int i = 0; i < 60; i++) Result2.data[i] = Result2.data[i + 4];
                    Result2.num_of_bits -= 4;
                    Result2.Print();
        }
        else if(Result3.num_of_bits == 60 && Result3.data[7] == Result3.data[3] &&
                Result3.data[6] == Result3.data[2] && Result3.data[5] == Result3.data[1] && 
                Result3.data[4] == Result3.data[0]) {
                    for(int i = 0; i < 60; i++) Result3.data[i] = Result3.data[i + 4];
                    Result3.num_of_bits -= 4;
                    Result3.Print();
           }
        else if(Result4.num_of_bits == 60 && Result4.data[7] == Result4.data[3] &&
                Result4.data[6] == Result4.data[2] && Result4.data[5] == Result4.data[1] && 
                Result4.data[4] == Result4.data[0]) {
                    for(int i = 0; i < 60; i++) Result4.data[i] = Result4.data[i + 4];
                    Result4.num_of_bits -= 4;
                    Result4.Print();
        }
        else cout << "ERROR!\n";
        
    return 0;
}

BigNumber Find_SQR(BigNumber a, BigNumber P){
    BigNumber power, SQR, CP, d;
    SQR = 1;
    d = 1;
    CP = a;
    if(P.data[0] % 4 == 3){                                         // case I : P = 3 (mod 4)
        power = (P + 1) / 4;
        SQR = fastpower(a, power, P);                               // SQR = a ^ ((p + 1) / 4) (mod P)
    }
    else if(P.data[0] % 8 == 5){                                    // case II : P = 5 (mod 8)
        power = (P - 1) / 4;
        d = fastpower(a, power, P);                                 // d = a ^ ((p - 1) / 4) (mod P)
        if(d == 1){                                                 // case I : d = 1
            power = (P + 3) / 8;
            SQR = fastpower(a, power, P);                           // SQR = a ^ ((p + 3) / 4) (mod P)
        }
        else if(d == -1){                                           // case II : d = -1
            power = (P + 3) / 8;
            SQR = fastpower(a * 4, power, P);                       // SQR = 0.5 * (4a) ^ ((p + 3) / 4) (mod P)
            SQR = SQR / 2;
        }
        else cout << "ERROR!\n";
    }
    else cout << "ERROR Input!\n";
    return SQR;
}

BigNumber fastpower(BigNumber a, BigNumber power, BigNumber N){
    BigNumber CP = a;
    BigNumber result = 1;
    while(!(power == 0)){
        if(power.data[0] % 2 == 1) result = (result * CP) % N;
        power = power / 2;
        CP = (CP * CP) % N;
    }
    return result;
}

// Extended Euclidean Algorithm
void EEA(BigNumber N, BigNumber a, BigNumber b, BigNumber& s0, BigNumber& s1, BigNumber& t0, BigNumber& t1){
    BigNumber q;
    BigNumber tmps, tmpt, tmp;
    tmp = a % b;
    if(tmp == 0) return;
    else{
        q = a / b;                                                  // q = a / b
        a = b;                                                      // a = b
        b = tmp;                                                    // b = a - q * b
        tmps = s0 - q * s1;                                         // s2 = s0 - q * s1
        if(tmps.sign == 1) tmps = tmps % N;
        else{
            tmps.sign = 1;
            tmps = tmps % N;
            tmps = N - tmps;
        }
        tmpt = t0 - q * t1;                                         // t2 = t0 - q * t1
        if(tmpt.sign == 1) tmpt = tmpt % N;
        else{
            tmpt.sign = 1;
            tmpt = tmpt % N;
            tmpt = N - tmpt;
        }
        s0 = s1; t0 = t1;
        s1 = tmps; t1 = tmpt;
        EEA(N, a, b, s0, s1, t0, t1);
    }
}

// For a odd prime P, P - 1 = m * (2 ^ k), if x belongs to Zp*, then a ^ m = 1 or a ^ (m * (2 ^ i)) = -1 for some i = [0, k - 1]
bool Miller_Rabin(BigNumber Prime, int iteration){
    BigNumber a = Prime;
    BigNumber m = Prime - 1;
    int k = 0;
    while(m.data[0] % 2 == 0) {                                     // Prime - 1 = m * (2 ^ k)
        k = k + 1;
        m = m / 2;
    }
    int len = Prime.num_of_bits;
    while(iteration > 0){
        //cout << iteration << endl;
        iteration--;
        a.num_of_bits = len;
        a.data[len - 1] = rand() % (MAX - 1) + 1;
        for(int i = 0; i < len - 1; i++) a.data[i] = rand() % MAX;
        a = a % (Prime - 4) + 2;                                    // choose random number a in [2, Prime - 2]
        a = fastpower(a, m, Prime);                                 // a = a ^ m (mod Prime)
        if(a == 1 || a == Prime - 1) continue;                      // Probably Prime (strong liar)
        else{
            bool skip = false;
            int j = 1;
            while(j < k){                                           // check (a ^ m) ^ j for j = 1 ~ k - 1
                a = (a * a) % Prime;
                if(a == Prime - 1) {                                // Probably Prime (strong liar)
                    skip = true;
                    break;
                }
                j = j + 1;
            }
            if(!skip) return false;                                 // Composite
        }
    }
    return true;                                                    // Probably Prime
}