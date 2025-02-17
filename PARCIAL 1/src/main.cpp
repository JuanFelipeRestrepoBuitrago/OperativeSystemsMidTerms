#include <iostream>
#include <cstring>
#include "./core/RSA.h"
#include "./core/Huffman.h"
#include "./helpers/Utils.h"
#include "./helpers/FileManager.h"
#include <cstring> 
#include <cstdlib>

// Default file paths for keys and messages
const char* defaultPublicKeyPath = "keys/public_key.txt";
const char* defaultPrivateKeyPath = "keys/private_key.txt";
const char* encryptedMessagePath = "messages/encrypted_message.txt";
const char* compressedMessagePath = "messages/compressed_message.bin";
const char* decompressedMessagePath = "messages/decompressed_message.txt";
const char* decryptedMessagePath = "messages/decrypted_message.txt";
const char* huffmanTablePath = "messages/huffman_table.txt";

void printUsage(const char* programName) {
    /**
     * Function to print the correct usage of the program
     * 
     * @param programName: The name of the program
     * 
     * @return: None
     */
    std::cout << "\nUsage: " << programName << " --[option] [arguments]\n\n";
    std::cout << "Try '" << programName << " --help' for more information.\n" << std::endl;
}

void printHelp(const char* programName) {
    /**
     * Function to print the help message with available options
     * 
     * @param programName: The name of the program
     * 
     * @return: None
     */
    std::cout << "\nUsage: " << programName << " --[option] [arguments]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help, -h         Show this help message\n";
    std::cout << "  --version, -v      Show RSA function version\n";
    std::cout << "  --generate         Generate public and private RSA keys\n";
    std::cout << "  --skeys            Show stored public and private keys\n";
    std::cout << "  --encrypt, -e      Encrypt a message file using a public key\n";
    std::cout << "  --compress, -c     Compress a file\n";
    std::cout << "  --decompress, -x   Decompress a file\n";
    std::cout << "  --decrypt, -d      Decrypt a message file using a private key\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " --generate\n";
    std::cout << "  " << programName << " --encrypt messages/original_message.txt keys/public_key.txt\n";
    std::cout << "  " << programName << " --compress messages/encripted_message.txt\n";
    std::cout << "  " << programName << " --decompress messages/compressed_message.bin\n";
    std::cout << "  " << programName << " --decrypt messages/decompressed_message.txt keys/private_key.txt\n";
    std::cout << std::endl;
}

void printVersion() {
    /**
     * Function to print the version of the RSA encryption system
     * 
     * @return: None
     */
    std::cout << "RSA function version 1.0" << std::endl;
}

std::string readTextFileAsString(const char* filePath) {
    /**
     * Function to read the contents of a file and return it as a string
     * 
     * @param filePath: The path of the file to be read
     * 
     * @return: A string containing the file contents
     */
    std::vector<char> content = FileManager::readTextFile(filePath);
    return std::string(content.begin(), content.end());
}

int main(int argc, char* argv[]) {
    /**
     * Main function of the program
     * 
     * @param argc: The number of command-line arguments
     * @param argv: The command-line arguments array
     * 
     * @return: Exit status (0 for success, 1 for errors)
     */
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    const char* prime1_env = std::getenv("PRIME1");
    const char* prime2_env = std::getenv("PRIME2");

    if (!prime1_env || !prime2_env) {
        std::cerr << "Error: Environment variables PRIME1 and PRIME2 are not set." << std::endl;
        return 1;
    }

    int PRIME1, PRIME2;
    try {
        PRIME1 = std::stoi(prime1_env);
        PRIME2 = std::stoi(prime2_env);
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid values for PRIME1 and PRIME2 environment variables." << std::endl;
        return 1;
    }

    Rsa rsa_management(PRIME1, PRIME2);
    Huffman huffman;

    std::string option = argv[1];

    if (option == "--help" || option == "-h") {
        printHelp(argv[0]);
        return 0;
    } 
    
    else if (option == "--version" || option == "-v") {
        printVersion();
        return 0;
    }

    else if (option == "--generate") {
        /**
         * Generate RSA public and private keys and save them to files
         */
        ResultGenerateKeys keys = rsa_management.generateKeys();
        FileManager::writeTextFile(defaultPublicKeyPath, 
            std::vector<char>(keys.publicKey, keys.publicKey + std::strlen(keys.publicKey)));
        FileManager::writeTextFile(defaultPrivateKeyPath, 
            std::vector<char>(keys.privateKey, keys.privateKey + std::strlen(keys.privateKey)));
        std::cout << "Public and private keys generated successfully." << std::endl;
        return 0;
    } 

    else if (option == "--skeys") {
        /**
         * Retrieve and display stored public and private keys
         */
        std::string privateKey = readTextFileAsString(defaultPrivateKeyPath);
        std::string publicKey = readTextFileAsString(defaultPublicKeyPath);
        std::cout << "Private Key: " << privateKey << "\nPublic Key: " << publicKey << std::endl;
        return 0;
    }

    else if (option == "--encrypt" || option == "-e") {
        /**
         * Encrypt a message file using the provided public key
         */
        if (argc < 4) {
            printUsage(argv[0]);
            return 1;
        }
        std::string message = readTextFileAsString(argv[2]);
        std::string publicKey = readTextFileAsString(argv[3]);
        char* encryptedMessage = rsa_management.encrypt(message.c_str(), publicKey.c_str());
        
        FileManager::writeTextFile(encryptedMessagePath,
            std::vector<char>(encryptedMessage, encryptedMessage + std::strlen(encryptedMessage)));
        std::cout << "Message encrypted successfully in " << encryptedMessagePath << std::endl;

        return 0;
    }

    else if (option == "--compress" || option == "-c") {
        /**
         * Compress a file using Huffman encoding
         */
        if (argc < 3) {
            printUsage(argv[0]);
            return 1;
        }
    
        std::vector<char> messageContent = FileManager::readTextFile(argv[2]);
        std::unordered_map<char, int> freqMap = Utils::createFreqMap(messageContent);
        huffman.buildTree(freqMap);
        std::vector<char> compressedMessage = huffman.compress(messageContent);
        
        std::unordered_map<std::string, char> reverseCodes = huffman.getReverseCodes();
        Utils::saveHuffmanTable(reverseCodes, huffmanTablePath);

        FileManager::writeBinaryFile(compressedMessagePath, compressedMessage);
        std::cout << "Message compressed successfully in " << compressedMessagePath << std::endl;
        
        return 0;
    }

    else if (option == "--descompress" || option == "-x") {
        /**
         * Decompress a file using Huffman encoding
         */
        if (argc < 3) {
            printUsage(argv[0]);
            return 1;
        }
    
        std::vector<char> messageContent = FileManager::readBinaryFile(argv[2]);
        std::unordered_map<std::string, char> reverseCodes = Utils::loadHuffmanTable(huffmanTablePath);
        std::vector<char> decompressedMessage = huffman.uncompress(messageContent, &reverseCodes);

        FileManager::writeTextFile(decompressedMessagePath, decompressedMessage);
        std::cout << "Message decompressed successfully in " << decompressedMessagePath << std::endl;
    
        return 0;
    }

    else if (option == "--decrypt" || option == "-d") {
        /**
         * Decrypt an encrypted message file using the private key
         */
        if (argc < 4) {
            printUsage(argv[0]);
            return 1;
        }
        std::string encryptedMessage = readTextFileAsString(argv[2]);
        std::string privateKey = readTextFileAsString(argv[3]);
        char* decryptedMessage = rsa_management.decrypt(encryptedMessage.c_str(), privateKey.c_str());

        FileManager::writeTextFile(decryptedMessagePath, 
            std::vector<char>(decryptedMessage, decryptedMessage + std::strlen(decryptedMessage)));
        std::cout << "Message decrypted successfully in " << decryptedMessagePath << std::endl;

        return 0;
    }

    return 0;
}