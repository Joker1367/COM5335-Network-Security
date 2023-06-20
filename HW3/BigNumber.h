#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#define MAX 16
#define MAX_Bit 1000

using namespace std;

class BigNumber{
public:
    int sign;
    int num_of_bits;
    vector<int> data;
    //constructors
    BigNumber(); 
    BigNumber(int); //directly convert from an int
    BigNumber(int, int);
    BigNumber(int, int, vector<int>);
    
    //overloaded arithmetic operators as member functions
    BigNumber operator+(BigNumber);
    BigNumber operator-(BigNumber);
    BigNumber operator*(BigNumber);
    BigNumber operator/(BigNumber); //integer division: 3/2==1
    BigNumber operator%(BigNumber);
    bool operator<(BigNumber);
    bool operator>(BigNumber);
    bool operator>=(BigNumber);
    bool operator==(BigNumber);

    BigNumber left_shift();
    BigNumber right_shift();

    //interface functions
    void Print();
    void GetData();
};
BigNumber::BigNumber(){
    sign = 1;
    num_of_bits = 1;
    data.assign(MAX_Bit, 0);
    for(int i = 0; i < MAX_Bit; i++) data[i] = 0;
}

BigNumber::BigNumber(int sgn, int length){
    sign = sgn;
    num_of_bits = length;
    data.assign(MAX_Bit, 0);
}

BigNumber::BigNumber(int sgn, int length, vector<int> d){
    sign = sgn;
    num_of_bits = length;
    data.assign(MAX_Bit, 0);
    for(int i = 0; i < length; i++) data[i] = d[i];
}

BigNumber::BigNumber(int x){
    if(x >= 0) sign = 1;
    else sign = -1;

    data.assign(MAX_Bit, 0);

    if(x == 0) num_of_bits = 1;
    else{
        x *= sign;
        num_of_bits = 0;
        while(x > 0){
            data[num_of_bits] = x % MAX;
            x /= MAX;
            num_of_bits++;
        }
    }
}

bool BigNumber::operator<(BigNumber y){
    BigNumber tmp(this->sign, this->num_of_bits, this->data);
    if(tmp.num_of_bits > y.num_of_bits) return false;
    else if(tmp.num_of_bits < y.num_of_bits) return true;
    else{
        int i = tmp.num_of_bits - 1;
        while(i > 0 && tmp.data[i] == y.data[i]) i--;
        if(tmp.data[i] >= y.data[i]) return false;
        else return true;
    }
}

bool BigNumber::operator>(BigNumber y){
   BigNumber tmp(this->sign, this->num_of_bits, this->data);
    if(tmp.num_of_bits > y.num_of_bits) return true;
    else if(tmp.num_of_bits < y.num_of_bits) return false;
    else{
        int i = tmp.num_of_bits - 1;
        while(i > 0 && tmp.data[i] == y.data[i]) i--;
        if(tmp.data[i] > y.data[i]) return true;
        else return false;
    }
}

BigNumber BigNumber::left_shift(){
    BigNumber tmp(this->sign, this->num_of_bits, this->data);
    if(tmp == 0) return tmp;
    tmp.num_of_bits++;
    for(int i = tmp.num_of_bits - 1; i > 0; i--) tmp.data[i] = tmp.data[i - 1];
    tmp.data[0] = 0;
    return tmp;
}

BigNumber BigNumber::right_shift(){
    BigNumber tmp(this->sign, this->num_of_bits, this->data);
    if(tmp.num_of_bits == 1) return 0;
    for(int i = 0; i < tmp.num_of_bits; i++) tmp.data[i] = tmp.data[i + 1];
    tmp.num_of_bits--;
    return tmp;
}

bool BigNumber::operator>=(BigNumber y){
   BigNumber tmp(this->sign, this->num_of_bits, this->data);
    if(tmp.num_of_bits > y.num_of_bits) return true;
    else if(tmp.num_of_bits < y.num_of_bits) return false;
    else{
        int i = tmp.num_of_bits - 1;
        while(i > 0 && tmp.data[i] == y.data[i]) i--;
        if(tmp.data[i] >= y.data[i]) return true;
        else return false;
    }
}

bool BigNumber::operator==(BigNumber y){
    BigNumber tmp(this->sign, this->num_of_bits, this->data);
    if(tmp.sign == y.sign && tmp.num_of_bits == y.num_of_bits){
        for(int i = 0; i < tmp.num_of_bits; i++){
            if(y.data[i] != tmp.data[i]) return false;
        }
        return true;
    }
    else return false;
}

BigNumber BigNumber::operator+(BigNumber y){
    BigNumber x(this->sign, this->num_of_bits, this->data);
    int i, carry;
    int x_len = x.num_of_bits;
    int y_len = y.num_of_bits;
    for(i = 0, carry = 0; i < x_len || i < y_len || carry > 0; i++){
        carry += (i < x_len) * this->data[i] + (i < y_len) * y.data[i];
        x.data[i] = carry % MAX;
        carry = carry / MAX;
    }
    x.num_of_bits = i;
    return x;
}

BigNumber BigNumber::operator-(BigNumber y){
    BigNumber x(this->sign, this->num_of_bits, this->data);
    int i, carry;
    int x_len = x.num_of_bits;
    int y_len = y.num_of_bits;
    for(i = 0, carry = 0; i < x_len || i < y_len || carry != 0; i++){
        carry += (i < x_len) * this->data[i] - (i < y_len) * y.data[i];
        x.data[i] = carry % MAX;
        carry = carry / MAX;
    }
    x.num_of_bits = i;
    for(;x.num_of_bits > 1 && x.data[x.num_of_bits - 1] == 0; x.num_of_bits--);
    if(x.data[x.num_of_bits - 1] < 0) x.sign = -1;
    else x.sign = 1;
    if(x.sign == 1){
        for(int j = 0; j < x.num_of_bits; j++){
            if(x.data[j] < 0){
                x.data[j + 1]--;
                x.data[j] += MAX;
            }
        }
    }
    else{
        for(int j = 0; j < x.num_of_bits; j++) x.data[j] *= -1;
        for(int j = 0; j < x.num_of_bits; j++){
            if(x.data[j] < 0){
                x.data[j + 1]--;
                x.data[j] += MAX;
            }
        }
    }
    for(;x.num_of_bits > 1 && x.data[x.num_of_bits - 1] == 0; x.num_of_bits--);
    return x;
}

BigNumber BigNumber::operator*(BigNumber y){
    BigNumber x(this->sign, this->num_of_bits, this->data);
    BigNumber result;
    int x_len = x.num_of_bits;
    int y_len = y.num_of_bits;
    int r_len = 0;
    for(int i = 0; i < y_len; i++){
        for(int j = 0; j < x_len; j++) {
            result.data[i + j] += y.data[i] * x.data[j];
            result.data[i + j + 1] += result.data[i + j] / MAX;
            result.data[i + j] = result.data[i + j] % MAX;
        }
    }
    result.num_of_bits = x_len + y_len + 1;
    for(int i = 0; i < result.num_of_bits; i++){
        if(result.data[i] >= MAX){
            result.data[i + 1] += result.data[i] / MAX;
            result.data[i] %= MAX;
        }
    }
    for(;result.num_of_bits > 1 && result.data[result.num_of_bits - 1] == 0; result.num_of_bits--);
    return result;
}

BigNumber BigNumber::operator/(BigNumber y){
    BigNumber result, t;
    BigNumber x(this->sign, this->num_of_bits, this->data);
    if(x < y) return result;
    if(x == y) return BigNumber(1);

    int i = 0;
    int q;
    int r_len = 0;
    int x_len = x.num_of_bits;
    int y_len = y.num_of_bits;
    i = x_len - 1;
    while(t < y){
        t = t.left_shift();
        t.data[0] = x.data[i];
        i--;
    }
    while(true){
        q = 0;
        while(q < MAX){
            if(t >= y) {
                t = t - y;
                q++;
            }
            else break;
        }
        result.data[r_len] = q;
        r_len++;
        if(i >= 0){
            t = t.left_shift();
            t.data[0] = x.data[i];
            i--;
        }
        else break;
    }
    result.num_of_bits = r_len;
    for(int i = 0; i < r_len / 2; i++) swap(result.data[i], result.data[r_len - 1 - i]);
    return result;
}

BigNumber BigNumber::operator%(BigNumber y){
    BigNumber t;
    BigNumber x(this->sign, this->num_of_bits, this->data);
    if(x < y) return x;
    if(x == y) return t;

    int i = 0;
    int q;
    int r_len = 0;
    int x_len = x.num_of_bits;
    int y_len = y.num_of_bits;
    i = x_len - 1;
    while(t < y){
        t = t.left_shift();
        t.data[0] = x.data[i];
        i--;
    }
    while(true){
        q = 0;
        while(q < MAX){
            if(t >= y) {
                t = t - y;
                q++;
            }
            else break;
        }
        if(i >= 0){
            t = t.left_shift();
            t.data[0] = x.data[i];
            i--;
        }
        else break;
    }
    return t;
}

void BigNumber::Print(){
    if(sign == -1) std::cout << "-";
    for(int i = 0; i < num_of_bits; i++) {
        if(i != 0 && i % 8 == 0) std::cout << std::dec << " ";
        std::cout << std:: hex << data[num_of_bits - 1 - i];
    }
    std::cout << std::dec << std::endl;
}

void BigNumber::GetData(){
    std::cout << "Sign = " << sign << std::endl;
    std::cout << "# of bits = " << num_of_bits << std::endl;
    std::cout << "Data = ";
    for(int i = 0; i < num_of_bits; i++) std::cout << std::hex << data[i];
    std::cout << std::dec << std::endl;
}