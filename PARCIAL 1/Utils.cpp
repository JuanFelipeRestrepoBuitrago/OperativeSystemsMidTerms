#include "Utils.h"

using std::vector;

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

    // Calculate the number of padding bytes needed
    size_t padding = (4 - (len % 4)) % 4;

    // Create a padded copy of the input string
    char* paddedStr = new char[len + padding + 1]; // +1 for null terminator
    strcpy(paddedStr, str); // Copy the original string
    memset(paddedStr + len, 0, padding); // Pad with null characters
    paddedStr[len + padding] = '\0'; // Null-terminate the padded string

    for (size_t i = 0; i < len; i += 4) {
        // Convert 4 characters into an integer
        int C = (static_cast<unsigned char>(str[i]) << 24) |
                (static_cast<unsigned char>(str[i + 1]) << 16) |
                (static_cast<unsigned char>(str[i + 2]) << 8) |
                static_cast<unsigned char>(str[i + 3]);
        // Add the integer to the vector
        bytes.push_back(C);
    }

    // Free the padded string
    delete[] paddedStr;
    
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
        // Extract 4 bytes of the integer number and convert each byte into characters
        for (int i = 3; i >= 0; i--) {
            str[index++] = static_cast<char>((C >> (i * 8)) & 0xFF);
        }
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

    if (m == 1) return 0; // Anything mod 1 is always 0

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