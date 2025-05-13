#include <iostream>
#include <cstring>
#include "./core/RSA.h"
#include "./core/Huffman.h"
#include "./helpers/Utils.h"
#include "./helpers/FileManager.h"
#include <cstring> 
#include <cstdlib>
#include "../libs/json.hpp"
#include <regex>

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

json compress(std::vector<std::string> files, const char* publicKey, const char* privateKey, Rsa& rsa_management, Huffman& huffman) {
    // Create JSON object
    json jsonData;
    
    jsonData["public_key"] = publicKey;
    jsonData["private_key"] = privateKey;  
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

        // Convert the encrypted data to vector of characters
        std::vector<char> encryptedDataChars(encryptedData.begin(), encryptedData.end());

        std::unordered_map<char, int> freqMap = Utils::createFreqMap(encryptedDataChars);
        if (freqMap.empty()) {
            std::cerr << "Warning: Frequency map is empty for file " << files[i] << std::endl;
            continue;
        }
        huffman.buildTree(freqMap);
        std::vector<char> compressedData = huffman.compress(encryptedDataChars);
        if (compressedData.empty()) {
            std::cerr << "Warning: Failed to compress file " << files[i] << std::endl;
            continue;
        }
        // Convert the compressed data to a vector of uint8_t
        std::vector<uint8_t> compressedDataUint8(compressedData.begin(), compressedData.end());
        std::string encodedData = Utils::binaryToBase64(compressedDataUint8);

        std::unordered_map<std::string, char> reverseCodes = huffman.getReverseCodes();
        if (reverseCodes.empty()) {
            std::cerr << "Warning: Reverse codes are empty for file " << files[i] << std::endl;
            continue;
        }

        json fileEntry;
        // Save the name of the file removing ./ or ../ or any secuence of them with regex
        fileEntry["file_name"] = std::regex_replace(files[i], std::regex(R"(\.{1,2}[/])"), "");
        fileEntry["file_data"] = encodedData;
        fileEntry["huffman_table"] = json::array();

        for (const auto& pair : reverseCodes) {
            json tableEntry;
            tableEntry["letter"] = pair.second;
            tableEntry["code"] = pair.first;
            fileEntry["huffman_table"].push_back(tableEntry);
        }

        jsonData["files"].push_back(fileEntry);
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

        // Check the extension of the output file
        std::string outputFile = argv[3];
        if (outputFile.substr(outputFile.find_last_of(".") + 1) != "perzip") {
            std::cerr << "Error: Output file must have the '.perzip' extension." << std::endl;
            printUsage(argv[0]);
            return 1;
        }

        ResultGenerateKeys keys = rsa_management.generateKeys();

        std::vector<std::string> allFiles = FileManager::getAllFilestoProcess(argv[2]);

        if (allFiles.empty()) {
            std::cerr << "Error: No files found to process." << std::endl;
            return 1;
        }

        json jsonData = compress(allFiles, keys.publicKey, keys.privateKey, rsa_management, huffman);

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

    else if (option == "--show" || option == "-s") {
        /* code */
        if (argc < 3) {
            printUsage(argv[0]);
            return 1;
        }
        std::string inputFile = argv[2];

        ArchiveData archive = FileManager::loadJsonFile(inputFile);

        std::vector<std::string> file_names;
        for (size_t i = 0; i < archive.files.size(); i++) {
            file_names.push_back(archive.files[i].file_name);
        }
        std::sort(file_names.begin(), file_names.end());

        std::cout << "Files in " << inputFile << ":\n";
        for (const auto& file_name : file_names) {
            std::cout << "  - " << file_name << std::endl;
        }
    }

    else {
        std::cerr << "Error: Unknown option '" << option << "'." << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    return 0;
}