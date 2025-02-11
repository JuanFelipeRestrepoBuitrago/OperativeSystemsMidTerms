#ifndef RSA_H
#define RSA_H

#include <bits/stdc++.h>
#include <stdexcept>
#include <vector>

struct ResultGenerateKeys {
    char* publicKey;
    char* privateKey;
};

class RSA {
private:
    int p;
    int q;
    char* publicKey;  // Public key in String format
    char* privateKey;  // Private key in String format
public:
    RSA(int p, int q);
    ~RSA();
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