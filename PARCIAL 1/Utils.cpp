#include "Utils.h"

using std::vector;
using std::string;

vector<int> Utils::stringToC(const char* str) {
    /**
     * Function to convert a string to a vector of integers
     * 
     * @param str: The string to be converted
     * 
     * @return: The vector of integers representation of the input string
     */
    vector<int> bytes;
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i ++) {
        int C = static_cast<int>(str[i]);
        bytes.push_back(C);        
    }
    
    return bytes;
}

char* Utils::cToString(const vector<int>& bytes) {
    /**
     * Function to convert a vector of integers to a character array
     * 
     * @param bytes: The vector of integers to be converted
     * 
     * @return: The character array representation of the input vector
     */
    // Allocate memory for the character array
    char* str = new char[bytes.size() * 4 + 1];
    size_t index = 0;

    for (int C : bytes) {
        str[index++] = static_cast<char>(C);
    }

    str[index] = '\0'; // Null-terminate the string
    return str;
}

void Utils::freeCString(char* str) {
    /**
     * Function to free memory allocated by cToString
     * 
     * @param str: The character array to be freed
     */
    delete[] str;
}

int Utils::powerModulus(int base, int expo, int m) {
    /**
     * Function to compute base^expo mod m
     * 
     * @param base: The base value
     * @param expo: The exponent value
     * @param m: The modulus value
     * 
     * @return: The result of base^expo mod m
     */

    // if (m == 1) return 0; // Anything mod 1 is always 0

    int result = 1;
    // Base reduced to prevent overflow
    base = base % m;

    while (expo > 0) {
        // If expo is odd, multiply base with result
        if (expo & 1)
            result = (result * 1LL * base) % m;

        base = (base * 1LL * base) % m; // Square the base
        expo = expo / 2; // Divide the expo by 2
    }

    return result;
}

int Utils::modInverse(int e, int phi) {
    /**
     * Function to find modular inverse of e modulo phi(n) where 1 < e < phi(n) 
     * 
     * @param e: The number for which the inverse is to be found
     * @param phi: The modulus value
     * 
     * @return: The modular inverse of e modulo phi(n)
     */
    for (int d = 2; d < phi; d++) {
        // If e * d is congruent to 1 modulo phi, then d is the modular inverse of e
        if ((e * d) % phi == 1)
            return d;
    }
    return -1;
}


std::vector<uint8_t> Utils::serializeNumbers(const std::vector<int>& numbers) {
    std::vector<uint8_t> binaryData;
    for (int num : numbers) {
        // Serialize each integer into 4 bytes (big-endian format)
        for (int i = sizeof(int) - 1; i >= 0; --i) {
            binaryData.push_back((num >> (i * 8)) & 0xFF);
        }
    }
    return binaryData;
}



std::string Utils::binaryToBase64(const std::vector<uint8_t>& binaryData) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    BIO_push(b64, mem);

    // Disable line breaks in Base64 output
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    // Write binary data to BIO
    BIO_write(b64, binaryData.data(), binaryData.size());
    BIO_flush(b64);

    // Read the encoded data from BIO
    BUF_MEM* bufferPtr;
    BIO_get_mem_ptr(b64, &bufferPtr);
    std::string result(bufferPtr->data, bufferPtr->length);

    // Clean up
    BIO_free_all(b64);

    return result;
}

std::vector<uint8_t> Utils::base64ToBinary(const std::string& base64Str) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new_mem_buf(base64Str.data(), base64Str.size());
    BIO_push(b64, mem);

    // Disable line breaks in Base64 input
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    // Read the decoded data from BIO
    std::vector<uint8_t> binaryData(base64Str.size()); // Allocate enough space
    int len = BIO_read(b64, binaryData.data(), base64Str.size());

    // Resize to actual decoded length
    binaryData.resize(len);

    // Clean up
    BIO_free_all(b64);

    return binaryData;
}

std::vector<int> Utils::deserializeNumbers(const std::vector<uint8_t>& binaryData) {
    std::vector<int> numbers;
    size_t numInts = binaryData.size() / sizeof(int);
    numbers.reserve(numInts);

    for (size_t i = 0; i < binaryData.size(); i += sizeof(int)) {
        int value = 0;
        for (size_t j = 0; j < sizeof(int); ++j) {
            value = (value << 8) | binaryData[i + j];
        }
        numbers.push_back(value);
    }

    return numbers;
}

char* Utils::numbersToBase64(const std::vector<int>& numbers) {
    vector<uint8_t> binaryData = Utils::serializeNumbers(numbers);
    string base64Str = Utils::binaryToBase64(binaryData);

    char* base64CStr = new char[base64Str.size() + 1];
    strcpy(base64CStr, base64Str.c_str());

    return base64CStr;
}

std::vector<int> Utils::base64ToNumbers(const char* base64CStr) {
    string base64Str(base64CStr);
    vector<uint8_t> binaryData = Utils::base64ToBinary(base64Str);
    return Utils::deserializeNumbers(binaryData);
}