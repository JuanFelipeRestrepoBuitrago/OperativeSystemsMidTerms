#include "RSA.h"
#include "Utils.h"
#include "Utils.h"
#include "FileManager.h"


using std::__gcd;
using std::cout;
using std::endl;
using std::vector;

RSA::RSA(int p, int q) : p(p), q(q), publicKey(nullptr), privateKey(nullptr) {
    /**
     * Constructor for the RSA class
     * 
     * @param p: The first prime number
     * @param q: The second prime number
     * 
     * @return: None
     */
}

RSA::~RSA() {
    /**
     * Destructor for the RSA class
     * 
     * @return: None
     */
    freeKeys();
}

ResultGenerateKeys RSA::generateKeys() {
    /**
     * Function to generate the public and private keys
     * 
     * @return: A struct containing the public and private keys
     */
    int n, e;

    // Calculate the Euler's Totient Function of n
    n = this -> p * q;
    int phi = (this -> p - 1) * (this -> q - 1);

    // Choose e, where 1 < e < phi(n) and gcd(e, phi(n)) == 1
    for (e = 2; e < phi; e++) {
        if (__gcd(e, phi) == 1) {
            break;
        }
    }

    // Compute d such that e * d ≡ 1 (mod phi(n))
    int d = Utils::modInverse(e, phi);

    // Convert the keys to string format

    ResultGenerateKeys result;
    result.publicKey = Utils::cToString({e, n});
    result.privateKey = Utils::cToString({d, n});
    return result;
}

char* RSA::encrypt(const char* message, const char* publicKey) {
    /**
     * Function to encrypt a message using the public key
     * 
     * @param message: The message to be encrypted
     * @param publicKey: The public key to be used for encryption
     * 
     * @return: The encrypted message
     */
    if (publicKey == nullptr) {
        publicKey = this -> publicKey;
        if (publicKey == nullptr) {
            throw std::invalid_argument("No public key provided or set in the class.");
        }
    }

    vector<int> publicKeyValues = Utils::stringToC(publicKey);
    int e = publicKeyValues[0];
    int n = publicKeyValues[1];

    vector<int> messageValues = Utils::stringToC(message);
    
    vector<int> encryptedValues;

    for (int i = 0; i < messageValues.size(); i++) {
        encryptedValues.push_back(Utils::powerModulus(messageValues[i], e, n));
    }

    return Utils::cToString(encryptedValues);
}
