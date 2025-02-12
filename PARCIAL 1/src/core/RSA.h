#ifndef RSA_H
#define RSA_H

#include "../helpers/Utils.h"
#include <bits/stdc++.h>
#include <stdexcept>
#include <vector>

using std::__gcd;
using std::cout;
using std::endl;
using std::vector;

struct ResultGenerateKeys {
    char* publicKey;
    char* privateKey;
};

class Rsa {
private:
    int p;
    int q;
    char* publicKey;  // Public key in String format
    char* privateKey;  // Private key in String format
public:
    Rsa(int p, int q);
    ~Rsa();
    ResultGenerateKeys generateKeys();
    char* encrypt(const char* message, const char* publicKey = nullptr);
    char* decrypt(const char* message, const char* privateKey = nullptr);
    char* getPublicKey();
    char* getPrivateKey();
    void setPublicKey(const char* publicKey);
    void setPrivateKey(const char* privateKey);
    void freeKeys();
};

#endif