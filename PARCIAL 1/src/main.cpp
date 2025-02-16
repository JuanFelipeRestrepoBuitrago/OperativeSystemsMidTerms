#include <iostream>
#include <cstring>
#include "./core/RSA.h"
#include "./core/Huffman.h"
#include "./helpers/Utils.h"
#include "./helpers/FileManager.h"
#include <cstring> 
#include <cstdlib>

const char* defaultPublicKeyPath = "keys/public_key.txt";
const char* defaultPrivateKeyPath = "keys/private_key.txt";
const char* encryptedMessagePath = "messages/encrypted_message.txt";
const char* compressedMessagePath = "messages/compressed_message.bin";
const char* decompressedMessagePath = "messages/decompressed_message.txt";
const char* decryptedMessagePath = "messages/decrypted_message.txt";
const char* huffmanTablePath = "messages/huffman_table.txt";

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
    std::cout << "  --compress, -c     Compress a file\n";
    std::cout << "  --descompress, -x  Descompress a file\n";
    std::cout << "  --decrypt, -d      Decrypt a message file using a private key\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " --generate\n";
    std::cout << "  " << programName << " --encrypt messages/original_message.txt keys/public_key.txt\n";
    std::cout << "  " << programName << " --compress messages/encripted_message.txt\n";
    std::cout << "  " << programName << " --descompress messages/compressed_message.bin\n";
    std::cout << "  " << programName << " --decrypt messages/decompressed_message.txt keys/private_key.txt\n";
    std::cout << std::endl;
}

void printVersion() {
    std::cout << "RSA function version 1.0" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    const char* prime1_env = std::getenv("PRIME1");
    const char* prime2_env = std::getenv("PRIME2");

    if (!prime1_env || !prime2_env) {
        std::cerr << "Error: Las variables de entorno PRIME1 y PRIME2 deben estar definidas." << std::endl;
        return 1;
    }

    int PRIME1 = std::stoi(prime1_env);
    int PRIME2 = std::stoi(prime2_env);

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
        ResultGenerateKeys keys = rsa_management.generateKeys();
        FileManager::writeFile(defaultPublicKeyPath, 
        std::vector<char>(keys.publicKey, keys.publicKey + std::strlen(keys.publicKey)));
        FileManager::writeFile(defaultPrivateKeyPath, 
        std::vector<char>(keys.privateKey, keys.privateKey + std::strlen(keys.privateKey)));
        std::cout << "Public and private keys generated successfully --------------" << std::endl;
        return 0;
    } 

    else if (option == "--skeys") {
        std::vector<char> privateKeyContent = FileManager::readFile(defaultPrivateKeyPath);
        std::string privateKey(privateKeyContent.begin(), privateKeyContent.end());
        std::cout << "Private key: " << privateKey << std::endl;
        std::vector<char> publicKeyContent = FileManager::readFile(defaultPublicKeyPath);
        std::string publicKey(publicKeyContent.begin(), publicKeyContent.end());
        std::cout << "Public key: " << publicKey << std::endl;
        return 0;
    }

    else if (option == "--encrypt" || option == "-e") {
        if (argc < 4) {
            printUsage(argv[0]);
            return 1;
        }
        std::vector<char> messageContent = FileManager::readFile(argv[2]);
        std::string message(messageContent.begin(), messageContent.end());

        std::vector<char> publicKeyContent = FileManager::readFile(argv[3]);
        std::string publicKey(publicKeyContent.begin(), publicKeyContent.end());
        
        char* encryptedMessage = rsa_management.encrypt(message.c_str(), publicKey.c_str());
        FileManager::writeFile(encryptedMessagePath, std::vector<char>(encryptedMessage, encryptedMessage + std::strlen(encryptedMessage)));
        std::cout << "Message of " << strlen(encryptedMessage) << " bytes encrypted successfully in " << encryptedMessagePath << std::endl;

        return 0;
    }

    else if (option == "--compress" || option == "-c") {
        if (argc < 3) {
            printUsage(argv[0]);
            return 1;
        }
    
        std::vector<char> messageContent = FileManager::readFile(argv[2]);
        std::unordered_map<char, int> freqMap = Utils::createFreqMap(messageContent);
        huffman.buildTree(freqMap);
        std::vector<char> compressedMessage = huffman.compress(messageContent);
        FileManager::writeFile(compressedMessagePath, compressedMessage);
        std::cout << "Message of " << compressedMessage.size() << " bytes compressed successfully in " << compressedMessagePath << std::endl;

        std::unordered_map<std::string, char> reverseCodes = huffman.getReverseCodes();
        Utils::saveHuffmanTable(reverseCodes, huffmanTablePath);
        return 0;
    }

    else if (option == "--descompress" || option == "-x") {
        if (argc < 3) {
            printUsage(argv[0]);
            return 1;
        }
    
        std::vector<char> messageContent = FileManager::readFile(argv[2]);
        std::unordered_map<std::string, char> reverseCodes = Utils::loadHuffmanTable(huffmanTablePath);
        std::vector<char> decompressedMessage = huffman.uncompress(messageContent, &reverseCodes);
        FileManager::writeFile(decompressedMessagePath, decompressedMessage);
        std::cout << "Message of " << decompressedMessage.size() << " bytes decompressed successfully in " << decompressedMessagePath << std::endl;
    
        return 0;
    }

    else if (option == "--decrypt" || option == "-d") {
        if (argc < 4) {
            printUsage(argv[0]);
            return 1;
        }
        std::vector<char> encryptedMessageContent = FileManager::readFile(argv[2]);
        std::string encryptedMessage(encryptedMessageContent.begin(), encryptedMessageContent.end());

        std::vector<char> privateKeyContent = FileManager::readFile(argv[3]);
        std::string privateKey(privateKeyContent.begin(), privateKeyContent.end());
        
        char* decryptedMessage = rsa_management.decrypt(encryptedMessage.c_str(), privateKey.c_str());
        FileManager::writeFile(decryptedMessagePath, std::vector<char>(decryptedMessage, decryptedMessage + std::strlen(decryptedMessage)));
        std::cout << "Message of " << strlen(decryptedMessage) << " bytes decrypted successfully in " << decryptedMessagePath << std::endl;

        return 0;
    }

    return 0;
}