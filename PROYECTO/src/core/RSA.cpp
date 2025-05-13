#include "RSA.h"
#include <numeric>

Rsa::Rsa(int p, int q) : p(p), q(q), publicKey(nullptr), privateKey(nullptr) {
    /**
     * Constructor for the Rsa class
     * 
     * @param p: The first prime number
     * @param q: The second prime number
     * 
     * @return: None
     */
}

Rsa::~Rsa() {
    /**
     * Destructor for the Rsa class
     * 
     * @return: None
     */
    freeKeys();
}

ResultGenerateKeys Rsa::generateKeys() {
    /**
     * Function to generate the public and private keys
     * 
     * @return: A struct containing the public and private keys
     */
    int n, e;

    // Calculate the Euler's Totient Function of n
    n = this -> p * this -> q;
    int phi = (this->p - 1) * (this->q - 1);

    // Choose e, where 1 < e < phi(n) and gcd(e, phi(n)) == 1
    for (e = 2; e < phi; e++) {
        if (std::__gcd(static_cast<unsigned int>(e), static_cast<unsigned int>(phi)) == 1) {
            break;
        }
    }

    // Compute d such that e * d ≡ 1 (mod phi(n))
    int d = Utils::modInverse(e, phi);

    // Convert the keys to string format
    ResultGenerateKeys result;
    result.publicKey = Utils::numbersToBase64({e, n});
    result.privateKey = Utils::numbersToBase64({d, n});
    return result;
}

std::vector<uint8_t> Rsa::encrypt(const std::vector<uint8_t>& data, const std::string& publicKeyStr) {
    if (publicKeyStr.empty()) {
        throw std::invalid_argument("No public key provided.");
    }

    std::vector<int> publicKeyValues = Utils::base64ToNumbers(publicKeyStr.c_str());
    if (publicKeyValues.size() != 2) {
        throw std::invalid_argument("Invalid public key format.");
    }
    int e = publicKeyValues[0];
    int n = publicKeyValues[1];

    // Ensure n is large enough to encrypt values up to 255
    if (n < 256) {
        throw std::invalid_argument("Modulus n is too small to encrypt byte values (must be >= 256).");
    }

    std::vector<uint8_t> encryptedValues;
    encryptedValues.reserve(data.size() * 4); // 4 bytes per encrypted value

    for (uint8_t byte : data) {
        // Encrypt each byte
        int encrypted = Utils::powerModulus(static_cast<int>(byte), e, n);
        if (encrypted >= n) {
            throw std::runtime_error("Encrypted value exceeds modulus n.");
        }
        // Store as 4 bytes (big-endian)
        encryptedValues.push_back(static_cast<uint8_t>(encrypted >> 24));
        encryptedValues.push_back(static_cast<uint8_t>(encrypted >> 16));
        encryptedValues.push_back(static_cast<uint8_t>(encrypted >> 8));
        encryptedValues.push_back(static_cast<uint8_t>(encrypted & 0xFF));
    }

    return encryptedValues;
}

std::vector<uint8_t> Rsa::decrypt(const std::vector<uint8_t>& data, const std::string& privateKeyStr) {
    if (privateKeyStr.empty()) {
        throw std::invalid_argument("No private key provided.");
    }

    if (data.size() % 4 != 0) {
        throw std::invalid_argument("Invalid encrypted data length (must be multiple of 4).");
    }

    std::vector<int> privateKeyValues = Utils::base64ToNumbers(privateKeyStr.c_str());
    if (privateKeyValues.size() != 2) {
        throw std::invalid_argument("Invalid private key format.");
    }
    int d = privateKeyValues[0];
    int n = privateKeyValues[1];

    std::vector<uint8_t> decryptedValues;
    decryptedValues.reserve(data.size() / 4);

    for (size_t i = 0; i < data.size(); i += 4) {
        // Reconstruct encrypted value from 4 bytes (big-endian)
        int encrypted = (static_cast<int>(data[i]) << 24) |
                        (static_cast<int>(data[i + 1]) << 16) |
                        (static_cast<int>(data[i + 2]) << 8) |
                        static_cast<int>(data[i + 3]);
        int decrypted = Utils::powerModulus(encrypted, d, n);
        // Ensure decrypted value is in byte range
        if (decrypted > 255) {
            std::cerr << "Warning: Decrypted value " << decrypted << " exceeds uint8_t range for n=" << n << "." << std::endl;
            decrypted = decrypted % 256; // Truncate to fit (temporary fix)
        }
        decryptedValues.push_back(static_cast<uint8_t>(decrypted));
    }

    return decryptedValues;
}

char* Rsa::getPublicKey() {
    /**
     * Function to get the public key
     * 
     * @return: The public key
     */
    return publicKey;
}

char* Rsa::getPrivateKey() {
    /**
     * Function to get the private key
     * 
     * @return: The private key
     */
    return privateKey;
}

void Rsa::setPublicKey(const char* publicKey) {
    /**
     * Function to set the public key
     * 
     * @param publicKey: The public key to be set
     * 
     * @return: None
     */
    this->publicKey = new char[strlen(publicKey) + 1];
    strcpy(this->publicKey, publicKey);
}

void Rsa::setPrivateKey(const char* privateKey) {
    /**
     * Function to set the private key
     * 
     * @param privateKey: The private key to be set
     * 
     * @return: None
     */
    this->privateKey = new char[strlen(privateKey) + 1];
    strcpy(this->privateKey, privateKey);
}

void Rsa::freeKeys() {
    /**
     * Function to free the memory allocated for the keys
     * 
     * @return: None
     */
    if (publicKey != nullptr) {
        delete[] publicKey;
        publicKey = nullptr;
    }

    if (privateKey != nullptr) {
        delete[] privateKey;
        privateKey = nullptr;
    }
}
