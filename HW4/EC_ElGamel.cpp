#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "BigNumber.h"

using namespace std;

typedef struct Point{
    BigNumber X = 0;
    BigNumber Y = 0;
} Point;

// Elliptic curve constant
// y^2 = x^3 + ax + b over GF(p) with base point G = (Gx, Gy)
BigNumber p(1, 40, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF");
BigNumber a(1, 40, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFC");
BigNumber b(1, 40, "1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45");
BigNumber Gx(1, 40, "4A96B5688EF573284664698968C38BB913CBFC82");
BigNumber Gy(1, 40, "23A628553168947D59DCC912042351377AC5FB32");
BigNumber n(1, 42, "0100000000000000000001F4C8F927AED3CA752257");
BigNumber mask(1, 41, "10000000000000000000000000000000000000000");

// Operation on Elliptic curve
Point Point_Inverse(Point P);
Point Point_Addition(Point P, Point Q);
Point Point_Doubling(Point P);
Point Point_scaling(Point P, BigNumber scalar);
Point Find_Point(BigNumber X);

// Functions for BigNumber Arithmetic
BigNumber Find_SQR(BigNumber a, BigNumber P);
BigNumber fastpower(BigNumber a, BigNumber power, BigNumber P);
BigNumber inverse(BigNumber X, BigNumber P);
BigNumber rounding(BigNumber X);
void EEA(BigNumber N, BigNumber a, BigNumber b, BigNumber& s0, BigNumber& s1, BigNumber& t0, BigNumber& t1);
bool is_on_curve(BigNumber Mx, BigNumber P);

int main(void){
    string s;

// EC-ElGamel Encryption
    // Input the Plaintext, Public Key Y and scalar nk
    cout << "<EC-ElGamal encryption>\n";
    cout << "Plaintext = ";
    getline(cin, s);
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    BigNumber Plaintext(1, 38, s);

    cout << "Pa = ";
    getline(cin, s);
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    BigNumber Y(1, 42, s);

    cout << "nk = ";
    getline(cin, s);
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    BigNumber nk(1, 40, s);

    // Find the correpsonding point for plaintext on the curve
    BigNumber Mx = Plaintext;                               // Mx = {Plaintext, 00}
    BigNumber My;
    Point Pm;
    Mx = Mx.left_shift();
    Mx = Mx.left_shift();
    while(!is_on_curve(Mx, p)) Mx = Mx + 1;                 // check whether on the curve, if not increment
    My = Find_SQR((Mx * Mx * Mx + a * Mx + b) % p, p);      // solve My according to the elliptic curve equation
    if(My.data[0] % 2 != 1) My = p - My;                    
    cout << "Mx = "; Mx.Print_fixed(40); cout << endl;
    cout << "My = "; My.Print_fixed(40); cout << endl;
    Pm.X = Mx; Pm.Y = My;                                   // Pm = point for Plaintext = {Mx, My}

    Point PG;                                               // PG = point for basepoint = {Gx, Gy} 
    PG.X = Gx; PG.Y = Gy;
    Point Pa = Point_scaling(PG, nk);                       // Pa = nk * PG

    Point Py = Find_Point(Y);                               // Py = point for public key Y
    Point Pb = Point_Addition(Pm, Point_scaling(Py, nk));   // Pb = Pm + nk * Py
    cout << "Cm = {Pk, Pb} = {";                            // Cm = {Pa, Pb}
    Pa.X.Print_fixed(40);
    cout << ", ";
    Pb.X.Print_fixed(40);
    cout << "}";
    cout << endl << endl;

// EC-ElGamel Decryption  
    // Input the ciphertext {K, B} and private key na
    cout << "<EC-ElGamal decryption>\n";
    cout << "Pk = ";
    getline(cin, s);
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    BigNumber K(1, 42, s);

    cout << "Pb = ";
    getline(cin, s);
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    BigNumber B(1, 42, s);

    cout << "na = ";
    getline(cin, s);
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    BigNumber na(1, 40, s);

    // Decryption Pm = Pb - x * Pa
    Pa = Find_Point(K);
    Pb = Find_Point(B);
    Pa = Point_scaling(Pa, na);
    Pa = Point_Inverse(Pa);
    Pm = Point_Addition(Pa, Pb);
    Pm.X = Pm.X.right_shift();
    Pm.X = Pm.X.right_shift();
    cout << "Plaintext M = "; Pm.X.Print_fixed(38);
    cout << endl;

    return 0; 
}

// Function for Finding point of a BigNumber on the elliptic curve
Point Find_Point(BigNumber X){
    BigNumber xp, yp, tmp;                                          // X = tmp || xp where tmp consist of the first two digits and xp be the rest of them
    Point P;
    xp = X;
    tmp = xp / mask;
    xp = xp % mask;

    BigNumber SQR = Find_SQR((xp * xp * xp + xp * a + b) % p, p);   // Find yp by the elliptic curve equation with xp as input
    if(tmp == 2 && SQR % 2 == 0) yp = SQR;                          // choose yp based on the method in sec-v2.pdf
    else if(tmp == 2 && SQR % 2 == 1) yp = p - SQR;
    else if(tmp == 3 && SQR % 2 == 0) yp = p - SQR;
    else if(tmp == 3 && SQR % 2 == 1) yp = SQR;
    else cout << "Invalid !!!";
    P.X = xp; P.Y = yp;
    return P;
}

// Point inverse : P = (x, y) -> -P = (x, -y)
Point Point_Inverse(Point P){
    Point res;
    res.X = P.X;
    res.Y = p - P.Y;
    return res;
}

// Point Addition : P = (x1, y1), Q = (x2, y2) -> P + Q = (x3, y3)
Point Point_Addition(Point P, Point Q){
    BigNumber numerator = rounding(Q.Y - P.Y) % p;                  // lambda = (y2 - y1) / (x2 - x1)
    BigNumber denominator = rounding(Q.X - P.X) % p;
    BigNumber lambda = (numerator * inverse(denominator, p)) % p;
    Point res;
    res.X = rounding(lambda * lambda - P.X - Q.X);                  // x3 = lambda ^ 2 - x1 - x2
    res.Y = rounding(lambda * rounding(P.X - res.X) - P.Y);         // y3 = -y1 + lambda * (x1 - x3)
    return res;
}

// Point Doubling : P = (x1, y1) -> 2P = (x2, y2)
// this is a group with characteristic != 2 or 3
Point Point_Doubling(Point P){
    BigNumber numerator = (P.X * P.X * 3 + a) % p;                  // lanbda = (3 * x1^2 + a) / (2 * y1)
    BigNumber denominator = (P.Y * 2) % p;
    BigNumber lambda = (numerator * inverse(denominator, p)) % p;
    Point res;
    res.X = rounding(lambda * lambda - P.X * 2);                    // x2 = lambda ^ 2 - 2 * x1
    res.Y = rounding(lambda * rounding(P.X - res.X) - P.Y);         // x3 = -y1 + lambda * (x1 - x2)
    return res;
}

// Use the idea of fast power to calculate nP where n is a BigNumber and P is a point on elliptic curve
Point Point_scaling(Point P, BigNumber scalar){
    Point res;
    res.X = -1, res.Y = -1;
    Point CP = P;
    while(!(scalar == 0)){
        if(scalar.data[0] % 2 == 1){
            if(res.X == -1) res = CP;
            else res = Point_Addition(res, CP);
        }
        scalar = scalar / 2;
        CP = Point_Doubling(CP);
    }
    return res;
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

// Check whehther there is a corresponding point for a BigNumber X on the elliptic curve by directly solving the equation 
bool is_on_curve(BigNumber Mx, BigNumber P){
    BigNumber value = (Mx * Mx * Mx + a * Mx + b) % P;
    BigNumber SQR = Find_SQR(value, P);
    SQR = (SQR * SQR) % P;
    if(SQR == value) return true;
    else return false;
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

BigNumber inverse(BigNumber X, BigNumber P){
    BigNumber s0, s1, t0, t1;
    s0 = 1; t1 = 1; s1 = 0; t0 = 0;
    EEA(P, P, X, s0, s1, t0, t1);
    return t1;
}

// A function that make sure all the BigNumber calculation result is still in the prime field
BigNumber rounding(BigNumber X){
    if(X.sign == -1){
        X.sign = 1;
        X = X % p;
        X = p - X;
    }
    else X = X % p;
    return X;
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