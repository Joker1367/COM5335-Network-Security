#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(void){
    bool find = false;
    vector<int> prime;
    prime.push_back(2);

    for(int i = 3; i < 1000; i++){
        find = true;
        for(int p: prime){
            if(i % p == 0) {
                find = false;
                break;
            }
        }
        if(find) prime.push_back(i);
    }
    for(int p: prime){
        cout << p << ", ";
    }
}