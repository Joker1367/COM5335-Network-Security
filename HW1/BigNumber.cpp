#include <iostream>
#include <stdio.h>
#include <string>
#include "BigNumber.h"

using namespace std;

int main(void){
    int data[100000] = {0};
    int len = 0;
    string a, b;
    cin >> a >> b;

    len = a.length();
    for(int i = 0; i < len; i++){
        if(a[i] >= 97) data[i] = a[i] - 97 + 10;
        else data[i] = a[i] - 48;
    }
    BigNumber X(1, len, data);

    len = b.length();
    for(int i = 0; i < len; i++){
        if(b[i] >= 97) data[i] = b[i] - 97 + 10;
        else data[i] = b[i] - 48;
    }
    BigNumber Y(1, len, data);

    cout << "a= ";
    X.Print();

    cout << "b= ";
    Y.Print();

    BigNumber Z;
    Z = X + Y;
    cout << "a+b = ";
    Z.Print();

    Z = X - Y;
    cout << "a-b = ";
    Z.Print();

    Z = X * Y;
    cout << "a*b = ";
    Z.Print();

    Z = X / Y;
    cout << "a/b = ";
    Z.Print();

    Z = X - (Y * Z);
    cout << "a%b = ";
    Z.Print();
}