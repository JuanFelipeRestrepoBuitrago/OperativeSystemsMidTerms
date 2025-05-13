#ifndef RSA_H
#define RSA_H

#include "../helpers/Utils.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include <numeric>


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
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data, const std::string& publicKey);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data, const std::string& privateKey);
    char* getPublicKey();
    char* getPrivateKey();
    void setPublicKey(const char* publicKey);
    void setPrivateKey(const char* privateKey);
    void freeKeys();
    void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const char* publicKey);
    void decryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const char* privateKey);

};

#endif