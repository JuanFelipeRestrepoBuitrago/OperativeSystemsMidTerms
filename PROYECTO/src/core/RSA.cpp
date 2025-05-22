#include "RSA.h"
#include <numeric>
#include <chrono>
#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif

Rsa::Rsa(int p, int q) : p(p), q(q), publicKey(nullptr), privateKey(nullptr)
{
    /**
     * Constructor for the Rsa class
     *
     * @param p: The first prime number
     * @param q: The second prime number
     *
     * @return: None
     */
}

Rsa::~Rsa()
{
    /**
     * Destructor for the Rsa class
     *
     * @return: None
     */
    freeKeys();
}

ResultGenerateKeys Rsa::generateKeys()
{
    /**
     * Function to generate the public and private keys
     *
     * @return: A struct containing the public and private keys
     */
    int n, e;

    // Calculate the Euler's Totient Function of n
    n = this->p * this->q;
    int phi = (this->p - 1) * (this->q - 1);

    // Choose e, where 1 < e < phi(n) and gcd(e, phi(n)) == 1
    for (e = 2; e < phi; e++)
    {
        if (std::__gcd(static_cast<unsigned int>(e), static_cast<unsigned int>(phi)) == 1)
        {
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

std::vector<uint8_t> Rsa::encrypt(const std::vector<uint8_t> &data, const std::string &publicKeyStr)
{
    /**
     * Function to encrypt the data using the public key
     *
     * @param data: The data to be encrypted
     * @param publicKeyStr: The public key in string format
     *
     * @return: The encrypted data
     */
    if (publicKeyStr.empty())
    {
        throw std::invalid_argument("❌ Error: No public key provided");
    }

    std::vector<int> publicKeyValues = Utils::base64ToNumbers(publicKeyStr.c_str());
    if (publicKeyValues.size() != 2)
    {
        throw std::invalid_argument("❌ Error: Invalid public key format");
    }
    int e = publicKeyValues[0];
    int n = publicKeyValues[1];

    // Ensure n is large enough to encrypt values up to 255
    if (n < 256)
    {
        throw std::invalid_argument("❌ Error: Modulus n is too small to encrypt byte values (must be >= 256)");
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> encryptedValues(data.size() * 4); // Pre-allocate the vector

#ifdef _OPENMP
    omp_set_num_threads(omp_get_max_threads());
#pragma omp parallel for
#endif
    for (size_t i = 0; i < data.size(); i++)
    {
#ifdef _OPENMP
        if (i == 0)
        {
            printf("\033[1;36m🔵 [OpenMP] Threads used for encryption: %d\033[0m\n", omp_get_num_threads());
        }
#endif
        uint8_t byte = data[i];
        int encrypted = Utils::powerModulus(static_cast<int>(byte), e, n);
        if (encrypted >= n)
        {
            throw std::runtime_error("❌ Error: Encrypted value exceeds modulus n");
        }
        size_t baseIndex = i * 4;
        encryptedValues[baseIndex] = static_cast<uint8_t>(encrypted >> 24);
        encryptedValues[baseIndex + 1] = static_cast<uint8_t>(encrypted >> 16);
        encryptedValues[baseIndex + 2] = static_cast<uint8_t>(encrypted >> 8);
        encryptedValues[baseIndex + 3] = static_cast<uint8_t>(encrypted & 0xFF);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printf("\033[1;32m🟢 [Timing] Encryption time: %lld ms\033[0m\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    return encryptedValues;
}

std::vector<uint8_t> Rsa::decrypt(const std::vector<uint8_t> &data, const std::string &privateKeyStr)
{
    /**
     * Function to decrypt the data using the private key
     *
     * @param data: The encrypted data to be decrypted
     * @param privateKeyStr: The private key in string format
     *
     * @return: The decrypted data
     */
    if (privateKeyStr.empty())
    {
        throw std::invalid_argument("❌ Error: No private key provided");
    }

    if (data.size() % 4 != 0)
    {
        throw std::invalid_argument("❌ Error: Invalid encrypted data length (must be multiple of 4)");
    }

    std::vector<int> privateKeyValues = Utils::base64ToNumbers(privateKeyStr.c_str());
    if (privateKeyValues.size() != 2)
    {
        throw std::invalid_argument("❌ Error: Invalid private key format");
    }
    int d = privateKeyValues[0];
    int n = privateKeyValues[1];

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> decryptedValues(data.size() / 4); // Pre-allocate the vector

#ifdef _OPENMP
    omp_set_num_threads(omp_get_max_threads());
#pragma omp parallel for
#endif
    for (size_t i = 0; i < data.size(); i += 4)
    {
#ifdef _OPENMP
        if (i == 0)
        {
            printf("\033[1;36m🔵 [OpenMP] Threads used for decryption: %d\033[0m\n", omp_get_num_threads());
        }
#endif
        int encrypted = (static_cast<int>(data[i]) << 24) |
                        (static_cast<int>(data[i + 1]) << 16) |
                        (static_cast<int>(data[i + 2]) << 8) |
                        static_cast<int>(data[i + 3]);
        int decrypted = Utils::powerModulus(encrypted, d, n);
        if (decrypted > 255)
        {
            std::cerr << "⚠️  Warning: Decrypted value " << decrypted << " exceeds uint8_t range for n=" << n << "\n"
                      << std::endl;
            decrypted = decrypted % 256;
        }
        decryptedValues[i / 4] = static_cast<uint8_t>(decrypted);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printf("\033[1;32m🟢 [Timing] Decryption time: %lld ms\033[0m\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    return decryptedValues;
}

char *Rsa::getPublicKey()
{
    /**
     * Function to get the public key
     *
     * @return: The public key
     */
    return publicKey;
}

char *Rsa::getPrivateKey()
{
    /**
     * Function to get the private key
     *
     * @return: The private key
     */
    return privateKey;
}

void Rsa::setPublicKey(const char *publicKey)
{
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

void Rsa::setPrivateKey(const char *privateKey)
{
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

void Rsa::freeKeys()
{
    /**
     * Function to free the memory allocated for the keys
     *
     * @return: None
     */
    if (publicKey != nullptr)
    {
        delete[] publicKey;
        publicKey = nullptr;
    }

    if (privateKey != nullptr)
    {
        delete[] privateKey;
        privateKey = nullptr;
    }
}
