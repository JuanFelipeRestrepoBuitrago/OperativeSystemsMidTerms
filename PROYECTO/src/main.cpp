#include <iostream>
#include <cstring>
#include "./core/RSA.h"
#include "./core/Huffman.h"
#include "./helpers/Utils.h"
#include "./helpers/FileManager.h"
#include <cstring> 
#include <cstdlib>
#include "../libs/json.hpp"

using json = nlohmann::json;

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
    std::cout << "  --compress, -c     Compress a file\n";
    std::cout << "  --decompress, -d   Decompress a file\n";
    std::cout << "  --show, -s       Show the inner files of a compressed file\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " --compress $INPUT_FILE $OUTPUT_FILE(the '.perzip' extension must be included in the $OUTPUTFILE)\n";
    std::cout << "  " << programName << " --decompress $INPUT_FILE $OUTPUT_FILE $REGEX_OF_FILES_TO_EXTRACT\n";
    std::cout << "  " << programName << " --show $INPUT_FILE\n";
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

json compress(std::vector<std::string> files, const char* publicKey, const char* privateKey, Rsa& rsa_management) {
    // Create JSON object
    json jsonData;
    
    jsonData["public_key"] = publicKey;
    jsonData["private_key"] = privateKey;  
    jsonData["huffman_table"] = json::array();
    jsonData["files"] = json::array();

    for (size_t i = 0; i < files.size(); i++) {        
        // Read the file
        std::vector<uint8_t> fileData = FileManager::readBinaryFile(files[i]);
        if (fileData.empty()) {
            std::cerr << "Warning: File " << files[i] << " is empty or could not be read." << std::endl;
            continue;
        }
        
        std::vector<uint8_t> encryptedData = rsa_management.encrypt(fileData, publicKey);
        if (encryptedData.empty()) {
            std::cerr << "Warning: Failed to encrypt file " << files[i] << std::endl;
            continue;
        }

        std::vector<uint8_t> decryptedData = rsa_management.decrypt(encryptedData, privateKey);
        if (decryptedData.empty()) {
            std::cerr << "Warning: Failed to decrypt file " << files[i] << std::endl;
            continue;
        }

        // Check if the decrypted data matches the original data
        if (fileData != decryptedData) {
            std::cerr << "Warning: Decrypted data does not match original data for file " << files[i] << std::endl;
            continue;
        }

        jsonData["files"].push_back({{"file_name", files[i]}, {"file_data", fileData}});
    }

    // Return the JSON object
    return jsonData;
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

    else if (option == "--compress" || option == "-c"){
        /* code */
        if (argc < 4) {
            printUsage(argv[0]);
            return 1;
        }

        ResultGenerateKeys keys = rsa_management.generateKeys();

        std::vector<std::string> allFiles = FileManager::getAllFilestoProcess(argv[2]);

        if (allFiles.empty()) {
            std::cerr << "Error: No files found to process." << std::endl;
            return 1;
        }

        json jsonData = compress(allFiles, keys.publicKey, keys.privateKey, rsa_management);

        if (FileManager::saveJsonFile(argv[3], jsonData)) {
            std::cout << "JSON file created successfully." << std::endl;
        } else {
            std::cerr << "Error: Failed to create JSON file." << std::endl;
            return 1;
        }
    }

    else if (option == "--decompress" || option == "-d") {
        /* code */
        if (argc < 4) {
            printUsage(argv[0]);
            return 1;
        }
    }

    else {
        std::cerr << "Error: Unknown option '" << option << "'." << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    return 0;
}