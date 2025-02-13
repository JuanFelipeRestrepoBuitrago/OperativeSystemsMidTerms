#include <iostream>
#include <fstream>
#include <cstring>
#include "src/core/RSA.h"
#include "src/helpers/Utils.h"
#include "src/helpers/FileManager.h"

#define PUBLIC_KEY_FILE "keys/public_key.txt"
#define PRIVATE_KEY_FILE "keys/private_key.txt"
#define ORIGINAL_MSG_FILE "messages/original_message.txt"
#define ENCRYPTED_MSG_FILE "messages/encrypted_message.txt"
#define DECRYPTED_MSG_FILE "messages/decrypted_message.txt"

void printUsage(const char* programName) {
    std::cout << "\nUsage: " << programName << " --[option] [arguments]\n\n";
    std::cout << "Try '" << programName << " --help' for more information.\n" << std::endl;
}

void printHelp(const char* programName) {
    std::cout << "\nUsage: " << programName << " --[option] [arguments]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help, -h         Show this help message\n";
    std::cout << "  --version, -v      Show RSA function version\n";
    std::cout << "  --generate         Generate public and private RSA keys\n";
    std::cout << "  --skeys            Show stored public and private keys\n";
    std::cout << "  --encrypt, -e      Encrypt a message file using a public key\n";
    std::cout << "  --decrypt, -d      Decrypt a message file using a private key\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " --generate\n";
    std::cout << "  " << programName << " --encrypt messages/original_message.txt keys/public_key.txt\n";
    std::cout << "  " << programName << " --decrypt messages/encrypted_message.txt keys/private_key.txt\n";
    std::cout << std::endl;
}

void printVersion() {
    std::cout << "RSA function version 1.0" << std::endl;
}

void saveKeysToFiles(const ResultGenerateKeys& keys) {
    std::ofstream pubFile(PUBLIC_KEY_FILE, std::ios::trunc);
    std::ofstream privFile(PRIVATE_KEY_FILE, std::ios::trunc);

    if (!pubFile || !privFile) {
        std::cerr << "Error: Failed to create key files." << std::endl;
        return;
    }

    pubFile << keys.publicKey << std::endl;
    privFile << keys.privateKey << std::endl;

    pubFile.close();
    privFile.close();
    std::cout << "Keys generated and saved in '" << PUBLIC_KEY_FILE << "' and '" << PRIVATE_KEY_FILE << "'.\n";
}

std::string readKeyFromFile(const std::string& filePath) {
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cerr << "Error: Unable to read key from " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string key;
    std::getline(inFile, key);
    inFile.close();
    return key;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    Rsa rsa_management(61, 53);  // Example with prime numbers

    std::string option = argv[1];

    if (option == "--help" || option == "-h") {
        printHelp(argv[0]);
        return 0;
    } else if (option == "--version" || option == "-v") {
        printVersion();
        return 0;
    } else if (option == "--generate") {
        // Generate RSA keys
        ResultGenerateKeys keys = rsa_management.generateKeys();
        saveKeysToFiles(keys);
        return 0;
    } else if (option == "--skeys") {
        // Show stored keys
        if (!std::ifstream(PUBLIC_KEY_FILE) || !std::ifstream(PRIVATE_KEY_FILE)) {
            std::cerr << "Error: Key files do not exist. Generate keys first using '--generate'.\n";
            return 1;
        }

        std::cout << "Public Key: " << readKeyFromFile(PUBLIC_KEY_FILE) << std::endl;
        std::cout << "Private Key: " << readKeyFromFile(PRIVATE_KEY_FILE) << std::endl;
        return 0;
    } else if (option == "--encrypt" || option == "-e") {
        if (argc < 4) {
            std::cerr << "Usage: " << argv[0] << " --encrypt [message-file] [public-key]\n";
            return 1;
        }
        std::string messageFile = argv[2];
        std::string publicKey = readKeyFromFile(argv[3]);

        // Encrypt and save
        std::vector<char> messageData = FileManager::readFile(messageFile);
        std::string message(messageData.begin(), messageData.end());

        char* encryptedMessage = rsa_management.encrypt(message.c_str(), publicKey.c_str());
        FileManager::writeFile(ENCRYPTED_MSG_FILE, std::vector<char>(encryptedMessage, encryptedMessage + strlen(encryptedMessage)));

        std::cout << "Encrypted message saved in '" << ENCRYPTED_MSG_FILE << "'\n";
        Utils::freeCString(encryptedMessage);
        return 0;
    } else if (option == "--decrypt" || option == "-d") {
        if (argc < 4) {
            std::cerr << "Usage: " << argv[0] << " --decrypt [message-file] [private-key]\n";
            return 1;
        }
        std::string encryptedFile = argv[2];
        std::string privateKey = readKeyFromFile(argv[3]);

        // Decrypt and save
        std::vector<char> encryptedData = FileManager::readFile(encryptedFile);
        std::string encryptedMessage(encryptedData.begin(), encryptedData.end());

        char* decryptedMessage = rsa_management.decrypt(encryptedMessage.c_str(), privateKey.c_str());
        FileManager::writeFile(DECRYPTED_MSG_FILE, std::vector<char>(decryptedMessage, decryptedMessage + strlen(decryptedMessage)));

        std::cout << "Decrypted message saved in '" << DECRYPTED_MSG_FILE << "'\n";
        Utils::freeCString(decryptedMessage);
        return 0;
    } else {
        std::cerr << "Unknown option: " << option << "\nUse --help for more information.\n";
        return 1;
    }

    return 0;
}
