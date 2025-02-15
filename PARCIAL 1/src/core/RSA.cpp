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
    n = this->p * q;
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

char* Rsa::encrypt(const char* message, const char* publicKey) {
    /**
     * Function to encrypt a message using the public key
     * 
     * @param message: The message to be encrypted
     * @param publicKey: The public key to be used for encryption
     * 
     * @return: The encrypted message
     */
    // If the public key is not provided and not set in the class, throw an error
    if (publicKey == nullptr) {
        publicKey = this->publicKey;
        if (publicKey == nullptr) {
            throw std::invalid_argument("No public key provided or set in the class.");
        }
    }

    // Convert the public key and message to integer values
    vector<int> publicKeyValues = Utils::base64ToNumbers(publicKey);
    vector<int> messageValues = Utils::stringToC(message);
    
    vector<int> encryptedValues;

    // Encrypt each character of the message
    for (size_t i = 0; i < messageValues.size(); i++) {
        encryptedValues.push_back(Utils::powerModulus(messageValues[i], publicKeyValues[0], publicKeyValues[1]));
    }

    // Convert the encrypted values to Base64 encoded string
    return Utils::numbersToBase64(encryptedValues);
}

char* Rsa::decrypt(const char* message, const char* privateKey) {
    /**
     * Function to decrypt a message using the private key
     * 
     * @param message: The message to be decrypted
     * @param privateKey: The private key to be used for decryption
     * 
     * @return: The decrypted message
     */
    // If the private key is not provided and not set in the class, throw an error
    if (privateKey == nullptr) {
        privateKey = this->privateKey;
        if (privateKey == nullptr) {
            throw std::invalid_argument("No private key provided or set in the class.");
        }
    }

    // Convert the private key and message to integer values
    vector<int> privateKeyValues = Utils::base64ToNumbers(privateKey);
    vector<int> messageValues = Utils::base64ToNumbers(message);
    
    vector<int> decryptedValues;

    // Decrypt each character of the message
    for (size_t i = 0; i < messageValues.size(); i++) {
        decryptedValues.push_back(Utils::powerModulus(messageValues[i], privateKeyValues[0], privateKeyValues[1]));
    }

    // Convert the decrypted values to a string
    return Utils::cToString(decryptedValues);
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
