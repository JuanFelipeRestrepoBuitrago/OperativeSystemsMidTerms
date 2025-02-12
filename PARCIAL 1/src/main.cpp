#include <iostream>
#include "RSA.h"

void printUsage(char* programName){
    std::cout << "\nUsage: " << programName << " --[option] [arguments]\n\n";
    std::cout<<"Try '"<< programName << " --help or -h' for more information" <<std::endl;
};

void printHelp(const char* programName) {
    std::cout << "\nUsage: " << programName << " --[option] [arguments]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help, -h         Show this help message\n";
    std::cout << "  --version, -v      Show RSA function version\n";
    std::cout << "  --generate         Generate public and private RSA keys\n";
    std::cout << "  --skeys            Show stored public and private keys\n";
    std::cout << "  --encrypt, -e      Encrypt a message file using a public key\n";
    std::cout << "                    Usage: " << programName << " --encrypt [message-file] [public-key]\n";
    std::cout << "  --decrypt, -d      Decrypt a message file using a private key\n";
    std::cout << "                    Usage: " << programName << " --decrypt [message-file] [private-key]\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " --generate\n";
    std::cout << "  " << programName << " --encrypt message.txt public_key.pem\n";
    std::cout << "  " << programName << " --decrypt encrypted.txt private_key.pem\n";
    std::cout << std::endl;
}

void printVersion() {
    std::cout << "RSA function version 1.0" << std::endl;
}


int main(int argc, char* argv[]) {
    RSA rsa_management(3,4);

    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    else if (argv[1] == "--help" || argv[1] == "-h") {
        printHelp(argv[0]);
        return 0;
    }
    else if (argv[1] == "--version" || argv[1] == "-v") {
        printVersion();
        return 0;
    }
    else if (argv[1] == "--generate") {
        // Generate public and private RSA keys
        return 0;
    }
    else if (argv[1] == "--skeys") {
        system("ls -l /");  // Cambia la ruta por la que necesites
        return 0;
    }
    else if (argv[1] == "--encrypt" || argv[1] == "-e") {
        if (argc < 4) {
            std::cout << "Usage: "<< argv[0] << "--encrypt [message-file] [public-key]"<<std::endl;
            return 1;
        }
        return 0;
    }
    else if (argv[1] == "--decrypt" || argv[1] == "-d") {
        // Decrypt a message file using a private key
        if (argc < 4) {
            printf("Usage: %s --decrypt [message-file] [private-key]\n", argv[0]);
            return 1;
        }
        return 0;
    }
    else {
        printf("Unknown option: %s\n", argv[1]);
        printf("Use --help for more information.\n");
        return 1;
    }


    return 0;
}
