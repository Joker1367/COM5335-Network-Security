#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "BigNumber.h"

using namespace std;

int main(void){
    vector<int> data(MAX_Bit, 0);
    int len = 0;
    string a, b;
    cin >> a >> b;
    cout << a << endl << b << endl;

    len = a.length();
    for(int i = 0; i < len; i++){
        if(a[i] >= 97) data[len - 1 - i] = a[i] - 97 + 10;
        else data[len - 1 - i] = a[i] - 48;
    }
    BigNumber X(1, len, data);
    for(int i = 0; i < len; i++) data[i] = 0;

    len = b.length();
    for(int i = 0; i < len; i++){
        if(b[i] >= 97) data[len - 1 - i] = b[i] - 97 + 10;
        else data[len - 1 - i] = b[i] - 48;
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

    Z = X % Y;
    cout << "a%b = ";
    Z.Print();
}