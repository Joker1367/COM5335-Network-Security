#include <iostream>
#include <stdlib.h>
#define MAX 16
#define MAX_Bit 100000
class BigNumber{
private:
    bool sign;
    int num_of_bits;
    int *data;
public:
    //constructors
    BigNumber(); 
    BigNumber(int); //directly convert from an int
    BigNumber(bool, int, int*);
    
    //overloaded arithmetic operators as member functions
    BigNumber operator+(BigNumber);
    BigNumber operator-(BigNumber);
    BigNumber operator*(BigNumber);
    BigNumber operator/(BigNumber); //integer division: 3/2==1
    BigNumber operator%(BigNumber);
    bool operator>=(BigNumber);

    //interface functions
    void Print();
    void GetData();
};
BigNumber::BigNumber(){
    sign = 1;
    num_of_bits = 1;
    data = new int[MAX_Bit];
    for(int i = 0; i < MAX_Bit; i++) data[i] = 0;
}

BigNumber::BigNumber(bool sgn, int length, int* number){
    data = new int[MAX_Bit];
    sign = sgn;
    num_of_bits = length;
    for(int i = 0; i < length; i++) {
        data[i] = number[i];
        //std::cout << std::hex << data[i];
        //std::cout << " ";
    }
}

BigNumber::BigNumber(int x){
    if(x >= 0) sign = 1;
    else sign = -1;

    data = new int[MAX_Bit];

    x *= sign;
    num_of_bits = 0;
    while(x > 0){
        data[num_of_bits] = x % MAX;
        x /= MAX;
        num_of_bits++;
    }
}

bool BigNumber::operator>=(BigNumber y){
    BigNumber tmp(this->sign, this->num_of_bits, this->data);
    tmp = tmp - y;
    if(!tmp.sign) return false;
    else return true;
}

BigNumber BigNumber::operator+(BigNumber y){
    BigNumber x(this->sign, this->num_of_bits, this->data);
    int i, carry;
    int x_len = x.num_of_bits;
    int y_len = y.num_of_bits;
    //std::cout << x_len << " " << y_len << std::endl;
    for(i = 0, carry = 0; i < x_len || i < y_len || carry > 0; i++){
        carry += (i < x_len) * this->data[x_len - 1 - i] + (i < y_len) * y.data[y_len - 1 - i];
        x.data[i] = carry % MAX;
        carry = carry / MAX;
        //std::cout << x.data[i] << " " << std::hex << x.data[i] << std::dec << std::endl;
    }
    x.num_of_bits = i;
    for(int j = 0; j < i / 2; j++) std::swap(x.data[j], x.data[i - 1 - j]);
    return x;
}

BigNumber BigNumber::operator-(BigNumber y){
    BigNumber x(this->sign, this->num_of_bits, this->data);
    int i, carry;
    int x_len = x.num_of_bits;
    int y_len = y.num_of_bits;
    for(i = 0, carry = 0; i < x_len || i < y_len || carry != 0; i++){
        carry += (i < x_len) * this->data[x_len - 1 - i] - (i < y_len) * y.data[y_len - 1 - i];
        x.data[i] = carry % MAX;
        carry = carry / MAX;
    }
    x.num_of_bits = i;
    for(;x.num_of_bits > 1 && x.data[x.num_of_bits - 1] == 0; x.num_of_bits--);
    if(x.data[x.num_of_bits - 1] < 0) x.sign = 0;
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
    for(int j = 0; j < x.num_of_bits / 2; j++) std::swap(x.data[j], x.data[x.num_of_bits - 1 - j]);
    return x;
}

BigNumber BigNumber::operator*(BigNumber y){
    BigNumber x(this->sign, this->num_of_bits, this->data);
    BigNumber result;
    int x_len = x.num_of_bits;
    int y_len = y.num_of_bits;
    int r_len, carry;
    for(int i = 0; i < x_len / 2; i++) std::swap(x.data[i], x.data[x_len - 1 - i]);
    for(int i = 0; i < y_len / 2; i++) std::swap(y.data[i], y.data[y_len - 1 - i]);
    for(int i = 0; i < y_len; i++){
        for(int j = 0; j < x_len; j++) {
            result.data[i + j] += y.data[i] * x.data[j];
            result.data[i + j + 1] += result.data[i + j] / MAX;
            result.data[i + j] = result.data[i + j] % MAX;
        }
    }
    r_len = x_len + y_len + 1;
    while(result.data[r_len] == 0) r_len--;
    result.num_of_bits = ++r_len;
    for(int i = 0; i < r_len / 2; i++) std::swap(result.data[i], result.data[r_len - 1 - i]);
    for(int i = 0; i < y_len / 2; i++) std::swap(y.data[i], y.data[y_len - 1 - i]);
    return result;
}

BigNumber BigNumber::operator/(BigNumber y){
    BigNumber x(this->sign, this->num_of_bits, this->data);
    BigNumber result;
    if(!(x >= y)) return result;
    int index = 0;
    x.num_of_bits = y.num_of_bits;
    for(int i = 0; i < x.num_of_bits; i++) x.data[i] = this->data[i];
    for(int i = y.num_of_bits; i <= this->num_of_bits; i++){
        while(x >= y){
            x = x - y;
            result.data[index]++;
        }
        index++;
        if(x.num_of_bits == 1 && x.data[0] == 0) x.data[0] = this->data[i];
        else {
            x.data[x.num_of_bits] = this->data[i];
            x.num_of_bits++;
        }
    }
    int redundant = 0;
    for(; redundant < index && result.data[redundant] == 0; redundant++);
    for(int i = 0; i < index - redundant; i++) result.data[i] = result.data[i + redundant];
    result.num_of_bits = index - redundant;
    return result;
}

BigNumber BigNumber::operator%(BigNumber y){
    BigNumber x(this->sign, this->num_of_bits, this->data);
    BigNumber tmp = x / y;
    x = x - (x / y) * y;
    return x;
}

void BigNumber::Print(){
    if(sign == 0) std::cout << "-";
    for(int i = 0; i < num_of_bits; i++) std::cout << std:: hex << data[i];
    std::cout << std::dec << std::endl;
}

void BigNumber::GetData(){
    std::cout << "Sign = " << sign << std::endl;
    std::cout << "# of bits = " << num_of_bits << std::endl;
    std::cout << "Data = ";
    for(int i = 0; i < num_of_bits; i++) std::cout << std::hex << data[i];
    std::cout << std::dec << std::endl;
}