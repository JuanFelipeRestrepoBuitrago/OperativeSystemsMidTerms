#include <iostream>
#include <cstring>
#include "./core/RSA.h"
#include "./helpers/Utils.h"
#include "./helpers/FileManager.h"
#include <cstring> 
#include <cstdlib>

#define PRIVATE_KEY_FILE "keys/private_key.txt"

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    int PRIME1 = std::stoi(std::getenv("PRIME1"));
    int PRIME2 = std::stoi(std::getenv("PRIME2"));

    Rsa rsa_management(PRIME1, PRIME2);

    std::string option = argv[1];

    if (option == "--help" || option == "-h") {
        printHelp(argv[0]);
        return 0;
    } else if (option == "--version" || option == "-v") {
        printVersion();
        return 0;
    }


    else if (option == "--generate") {
        const char* defaultPublicKeyPath;
        const char* defaultPrivateKeyPath;

        if (argc<2){
            defaultPublicKeyPath = "keys/public_key.txt";
            defaultPrivateKeyPath = "keys/private_key.txt";
        }
        else{
            if(argc==4){
                defaultPublicKeyPath = argv[2];
                defaultPrivateKeyPath = argv[3];
            }
            else{
                std::cerr << "Usage: " << argv[0] << " --generate [public-key-file] [private-key-file]\n";
                return 1;
            }
        }

        ResultGenerateKeys keys = rsa_management.generateKeys();
        FileManager::writeFile(defaultPublicKeyPath, 
            std::vector<char>(keys.publicKey, keys.publicKey + std::strlen(keys.publicKey)));
        FileManager::writeFile(defaultPrivateKeyPath, 
            std::vector<char>(keys.privateKey, keys.privateKey + std::strlen(keys.privateKey)));
        std::cout << "Public and private keys generated successfully --------------" << std::endl;
        return 0;
    } 

    return 0;
}
