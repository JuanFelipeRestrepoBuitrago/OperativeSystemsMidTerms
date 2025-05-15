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

json compress(const char* inputFile, const std::vector<std::string>& files, int prime1, int prime2) {
    /**
     * Function to compress and encrypt files using RSA and Huffman encoding
     * 
     * @param inputFile: The path of the input file to be compressed
     * @param files: A vector of file paths to be compressed and encrypted
     * @param prime1: The first prime number for RSA key generation
     * @param prime2: The second prime number for RSA key generation
     * 
     * @return: A JSON object containing the public key, private key, and compressed file data
     */
    Rsa rsa_management(prime1, prime2);

    // Create JSON object
    json jsonData;
    
    ResultGenerateKeys keys = rsa_management.generateKeys();
    jsonData["public_key"] = keys.publicKey;
    jsonData["private_key"] = keys.privateKey;
    jsonData["files"] = json::array();

    for (size_t i = 0; i < files.size(); i++) {
        Huffman huffman;     
        // Read the file
        std::vector<uint8_t> fileData = FileManager::readBinaryFile(files[i]);
        if (fileData.empty()) {
            std::cerr << "Warning: File " << files[i] << " is empty or could not be read." << std::endl;
            continue;
        }
        
        std::vector<uint8_t> encryptedData = rsa_management.encrypt(fileData, keys.publicKey);
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
        std::string inputFileRegex, fileName;

        if (std::regex_match(inputFile, std::regex(R"(\.{1,2}/?)"))) {
            fileName = std::regex_replace(files[i], std::regex(R"(\.{1,2}[/])"), "");
            inputFileRegex = "";
        } else {
            fileName = files[i];
            inputFileRegex = inputFile;
        }
        // Save the name of the file removing the inputFile path except the last part
        std::string lastPart = std::string(inputFileRegex).substr(std::string(inputFileRegex).find_last_of("/") + 1);

        fileEntry["file_name"] = std::regex_replace(fileName, std::regex(inputFileRegex), lastPart);
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

void decompress(const char* inputFile, const char* outputFile, std::string regexStr, int prime1, int prime2) {
    /**
     * Function to decompress and decrypt files using RSA and Huffman encoding
     * 
     * @param inputFile: The path of the input JSON file containing compressed data
     * @param outputFile: The path of the output directory to save decompressed files
     * @param regexStr: A regex string to filter files to be extracted
     * @param prime1: The first prime number for RSA key generation
     * @param prime2: The second prime number for RSA key generation
     * 
     * @return: None
     */
    Rsa rsa_management(prime1, prime2);

    // Load the JSON file
    ArchiveData archive = FileManager::loadJsonFile(inputFile);

    // Normalize regexStr
    bool withoutExternalFolder = false;
    if (regexStr.empty()) {
        regexStr = ""; // Default to match all
    } else {
        // Check for /* to set extractJustFiles
        if (regexStr.size() >= 2 && regexStr.substr(regexStr.size() - 2) == "/*") {
            withoutExternalFolder = true;
            regexStr = regexStr.substr(0, regexStr.size() - 2);
        }
        // Clean regex: remove leading ./, trailing /, replace standalone . with empty
        regexStr = std::regex_replace(regexStr, std::regex(R"(^\./|/$)"), "");
        if (regexStr == ".") {
            regexStr = "";
        }
    }

    // Decompress the files
    for (const auto& fileEntry : archive.files) {
        Huffman huffman;
        std::string fileName = fileEntry.file_name;

        // Check the file name contains the regex
        if (!std::regex_search(fileName, std::regex(regexStr))) {
            continue;
        }

        // Replace the matching part of the file name with output_file/
        std::string outputFileName = outputFile;
        if (regexStr.empty() || (!withoutExternalFolder && regexStr.substr(0, regexStr.find_last_of("/")) == regexStr)) {
            outputFileName += "/" + fileName;
        }
        else if (withoutExternalFolder) {
            outputFileName += std::regex_replace(fileName, std::regex(regexStr), "");
        } else {
            outputFileName += std::regex_replace(fileName, std::regex(regexStr.substr(0, regexStr.find_last_of("/"))), "");
        }

        std::string fileData = fileEntry.file_data;

        // Decode the Base64 data
        std::vector<uint8_t> decodedData = Utils::base64ToBinary(fileData.c_str());
        std::vector<char> decodedDataChars(decodedData.begin(), decodedData.end());

        // Decompress the data using Huffman
        std::unordered_map<std::string, char> reverseCodes = fileEntry.huffman_table;
        std::vector<char> decompressedData = huffman.uncompress(decodedDataChars, &reverseCodes);
        if (decompressedData.empty()) {
            std::cerr << "Warning: Failed to decompress file " << fileName << std::endl;
            continue;
        }
        std::vector<uint8_t> decompressedDataUint8(decompressedData.begin(), decompressedData.end());

        // Decrypt the data using RSA
        std::vector<uint8_t> decryptedData = rsa_management.decrypt(decompressedDataUint8, archive.private_key);
        if (decryptedData.empty()) {
            std::cerr << "Warning: Failed to decrypt file " << fileName << std::endl;
            continue;
        }
        // Create the output directory if it doesn't exist
        std::filesystem::path outputPath(outputFileName.substr(0, outputFileName.find_last_of("/")));
        if (!std::filesystem::exists(outputPath)) {
            std::filesystem::create_directories(outputPath);
        }

        if (!FileManager::writeBinaryFile(outputFileName, decryptedData)) {
            std::cerr << "Error: Failed to save decompressed file " << outputFileName << std::endl;
        }
    }
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

        // Check the input file exists
        if (!std::filesystem::exists(argv[2])) {
            std::cerr << "Error: Input file does not exist." << std::endl;
            return 1;
        }

        std::vector<std::string> allFiles = FileManager::getAllFilestoProcess(argv[2]);

        if (allFiles.empty()) {
            std::cerr << "Error: No files found to process." << std::endl;
            return 1;
        }

        json jsonData = compress(argv[2], allFiles, PRIME1, PRIME2);

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

        // Check if the output file is a directory
        if (!std::filesystem::is_directory(argv[3])) {
            std::cerr << "Error: Output file must be a directory." << std::endl;
            return 1;
        }
        std::string inputFile = argv[2];
        std::string outputFile = argv[3];
        std::string regexStr = argc > 4 ? argv[4] : "";

        decompress(inputFile.c_str(), outputFile.c_str(), regexStr, PRIME1, PRIME2);
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