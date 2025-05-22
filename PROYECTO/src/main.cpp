#include <iostream>
#include <cstring>
#include "./core/RSA.h"
#include "./core/Huffman.h"
#include "./helpers/Utils.h"
#include "./helpers/FileManager.h"
#include <cstdlib>
#include "../libs/json.hpp"
#include <regex>
#include <filesystem>

using json = nlohmann::json;

// ANSI color codes for better UX
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

// Emojis for visual feedback
#define CHECK_EMOJI "✅"
#define ERROR_EMOJI "❌"
#define INFO_EMOJI "ℹ️ "
#define FILE_EMOJI "📄"
#define FOLDER_EMOJI "📁"

void printUsage(const char *programName)
{
    /**
     * Function to print the correct usage of the program with enhanced formatting
     *
     * @param programName: The name of the program
     *
     * @return: None
     */
    std::cout << CYAN << "\n🔍 Usage: " << BOLD << programName << " --[option] [arguments]" << RESET << "\n\n";
    std::cout << YELLOW << "💡 Try '" << programName << " --help' for more information." << RESET << "\n"
              << std::endl;
}

void printHelp(const char *programName)
{
    /**
     * Function to print the help message with available options, using colors and emojis
     *
     * @param programName: The name of the program
     *
     * @return: None
     */
    std::cout << "===========================\n\n";
    std::cout << "🔧 Usage: " << BOLD << programName << " --[option] [arguments]" << RESET << "\n\n";
    std::cout << GREEN << "🛠️  Available Options:\n";
    std::cout << "  --help, -h         📖 Show this help message\n";
    std::cout << "  --version, -v      ℹ️  Show RSA function version\n";
    std::cout << "  --compress, -c     📦 Compress a file\n";
    std::cout << "  --decompress, -d   📥 Decompress a file\n";
    std::cout << "  --show, -s         👁️  Show the inner files of a compressed file\n";
    std::cout << "\n📝 Examples:\n";
    std::cout << "  " << programName << " --compress $INPUT_FILE $OUTPUT_FILE " << YELLOW << "(must include '.perzip' extension)" << RESET << "\n";
    std::cout << "  " << programName << " --decompress $INPUT_FILE $OUTPUT_FILE $REGEX_OF_FILES_TO_EXTRACT\n";
    std::cout << "  " << programName << " --show $INPUT_FILE\n";
    std::cout << RESET << std::endl;
}

void printVersion()
{
    /**
     * Function to print the version of the RSA encryption system with enhanced formatting
     *
     * @return: None
     */
    std::cout << GREEN << "🔐 RSA function version 1.0" << RESET << std::endl;
}

json compress(const char *inputFile, const std::vector<std::string> &files, int prime1, int prime2)
{
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
    std::cout << BLUE << "\n"
              << FILE_EMOJI << " Starting compression process..." << RESET << std::endl;
    Rsa rsa_management(prime1, prime2);
    json jsonData;

    ResultGenerateKeys keys = rsa_management.generateKeys();
    jsonData["public_key"] = keys.publicKey;
    jsonData["private_key"] = keys.privateKey;
    jsonData["files"] = json::array();

    for (size_t i = 0; i < files.size(); i++)
    {
        std::cout << CYAN << "  " << FILE_EMOJI << " Processing: " << files[i] << "..." << RESET << std::endl;
        Huffman huffman;
        std::vector<uint8_t> fileData = FileManager::readBinaryFile(files[i]);
        if (fileData.empty())
        {
            std::cerr << RED << ERROR_EMOJI << " Warning: File " << files[i] << " is empty or could not be read." << RESET << std::endl;
            continue;
        }

        std::vector<uint8_t> encryptedData = rsa_management.encrypt(fileData, keys.publicKey);
        if (encryptedData.empty())
        {
            std::cerr << RED << ERROR_EMOJI << " Warning: Failed to encrypt file " << files[i] << RESET << std::endl;
            continue;
        }

        std::vector<char> encryptedDataChars(encryptedData.begin(), encryptedData.end());
        std::unordered_map<char, int> freqMap = Utils::createFreqMap(encryptedDataChars);
        if (freqMap.empty())
        {
            std::cerr << RED << ERROR_EMOJI << " Warning: Frequency map is empty for file " << files[i] << RESET << std::endl;
            continue;
        }
        huffman.buildTree(freqMap);
        std::vector<char> compressedData = huffman.compress(encryptedDataChars);
        if (compressedData.empty())
        {
            std::cerr << RED << ERROR_EMOJI << " Warning: Failed to compress file " << files[i] << RESET << std::endl;
            continue;
        }
        std::vector<uint8_t> compressedDataUint8(compressedData.begin(), compressedData.end());
        std::string encodedData = Utils::binaryToBase64(compressedDataUint8); // Fixed: Changed base64ToBinary to binaryToBase64

        std::unordered_map<std::string, char> reverseCodes = huffman.getReverseCodes();
        if (reverseCodes.empty())
        {
            std::cerr << RED << ERROR_EMOJI << " Warning: Reverse codes are empty for file " << files[i] << RESET << std::endl;
            continue;
        }

        json fileEntry;
        std::string inputFileRegex, fileName;

        if (std::regex_match(inputFile, std::regex(R"(\.{1,2}/?)")))
        {
            fileName = std::regex_replace(files[i], std::regex(R"(\.{1,2}[/])"), "");
            inputFileRegex = "";
        }
        else
        {
            fileName = files[i];
            inputFileRegex = inputFile;
        }
        std::string lastPart = std::string(inputFileRegex).substr(std::string(inputFileRegex).find_last_of("/") + 1);

        fileEntry["file_name"] = std::regex_replace(fileName, std::regex(inputFileRegex), lastPart);
        fileEntry["file_data"] = encodedData;
        fileEntry["huffman_table"] = json::array();

        for (const auto &pair : reverseCodes)
        {
            json tableEntry;
            tableEntry["letter"] = pair.second;
            tableEntry["code"] = pair.first;
            fileEntry["huffman_table"].push_back(tableEntry);
        }

        jsonData["files"].push_back(fileEntry);
        std::cout << GREEN << CHECK_EMOJI << " Successfully processed: " << files[i] << RESET << std::endl;
    }

    std::cout << GREEN << CHECK_EMOJI << " Compression completed!" << RESET << std::endl;
    return jsonData;
}

void decompress(const char *inputFile, const char *outputFile, std::string regexStr, int prime1, int prime2)
{
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
    std::cout << BLUE << "\n"
              << FOLDER_EMOJI << " Starting decompression process..." << RESET << std::endl;
    Rsa rsa_management(prime1, prime2);
    ArchiveData archive = FileManager::loadJsonFile(inputFile);

    bool withoutExternalFolder = false;
    if (regexStr.empty())
    {
        regexStr = "";
    }
    else
    {
        if (regexStr.size() >= 2 && regexStr.substr(regexStr.size() - 2) == "/*")
        {
            withoutExternalFolder = true;
            regexStr = regexStr.substr(0, regexStr.size() - 2);
        }
        regexStr = std::regex_replace(regexStr, std::regex(R"(^\./|/$)"), "");
        if (regexStr == ".")
        {
            regexStr = "";
        }
    }

    for (const auto &fileEntry : archive.files)
    {
        std::cout << CYAN << "  " << FILE_EMOJI << " Decompressing: " << fileEntry.file_name << "..." << RESET << std::endl;
        Huffman huffman;
        std::string fileName = fileEntry.file_name;

        if (!std::regex_search(fileName, std::regex(regexStr)))
        {
            std::cout << YELLOW << "  Skipped: " << fileName << " (does not match regex)" << RESET << std::endl;
            continue;
        }

        std::string outputFileName = outputFile;
        if (regexStr.empty() || (!withoutExternalFolder && regexStr.substr(0, regexStr.find_last_of("/")) == regexStr))
        {
            outputFileName += "/" + fileName;
        }
        else if (withoutExternalFolder)
        {
            outputFileName += std::regex_replace(fileName, std::regex(regexStr), "");
        }
        else
        {
            outputFileName += std::regex_replace(fileName, std::regex(regexStr.substr(0, regexStr.find_last_of("/"))), "");
        }

        std::string fileData = fileEntry.file_data;
        std::vector<uint8_t> decodedData = Utils::base64ToBinary(fileData.c_str());
        std::vector<char> decodedDataChars(decodedData.begin(), decodedData.end());

        std::unordered_map<std::string, char> reverseCodes = fileEntry.huffman_table;
        std::vector<char> decompressedData = huffman.uncompress(decodedDataChars, &reverseCodes);
        if (decompressedData.empty())
        {
            std::cerr << RED << ERROR_EMOJI << " Warning: Failed to decompress file " << fileName << RESET << std::endl;
            continue;
        }
        std::vector<uint8_t> decompressedDataUint8(decompressedData.begin(), decompressedData.end());

        std::vector<uint8_t> decryptedData = rsa_management.decrypt(decompressedDataUint8, archive.private_key);
        if (decryptedData.empty())
        {
            std::cerr << RED << ERROR_EMOJI << " Warning: Failed to decrypt file " << fileName << RESET << std::endl;
            continue;
        }

        std::filesystem::path outputPath(outputFileName.substr(0, outputFileName.find_last_of("/")));
        if (!std::filesystem::exists(outputPath))
        {
            std::cout << YELLOW << "  Creating directory: " << outputPath << RESET << std::endl;
            std::filesystem::create_directories(outputPath);
        }

        if (!FileManager::writeBinaryFile(outputFileName, decryptedData))
        {
            std::cerr << RED << ERROR_EMOJI << " Error: Failed to save decompressed file " << outputFileName << RESET << std::endl;
        }
        else
        {
            std::cout << GREEN << CHECK_EMOJI << " Successfully decompressed: " << outputFileName << RESET << std::endl;
        }
    }
    std::cout << GREEN << CHECK_EMOJI << " Decompression completed!" << RESET << std::endl;
}

int main(int argc, char *argv[])
{
    /**
     * Main function of the program with improved UX
     *
     * @param argc: The number of command-line arguments
     * @param argv: The command-line arguments array
     *
     * @return: Exit status (0 for success, 1 for errors)
     */
    if (argc < 2)
    {
        std::cerr << RED << ERROR_EMOJI << " Error: No option provided." << RESET << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    const char *prime1_env = std::getenv("PRIME1");
    const char *prime2_env = std::getenv("PRIME2");

    if (!prime1_env || !prime2_env)
    {
        std::cerr << RED << ERROR_EMOJI << " Error: Environment variables PRIME1 and PRIME2 must be set." << RESET << std::endl;
        return 1;
    }

    int PRIME1, PRIME2;
    try
    {
        PRIME1 = std::stoi(prime1_env);
        PRIME2 = std::stoi(prime2_env);
    }
    catch (const std::exception &e)
    {
        std::cerr << RED << ERROR_EMOJI << " Error: Invalid values for PRIME1 or PRIME2 environment variables." << RESET << std::endl;
        return 1;
    }

    std::string option = argv[1];

    if (option == "--help" || option == "-h")
    {
        printHelp(argv[0]);
        return 0;
    }
    else if (option == "--version" || option == "-v")
    {
        printVersion();
        return 0;
    }
    else if (option == "--compress" || option == "-c")
    {
        if (argc < 4)
        {
            std::cerr << RED << ERROR_EMOJI << " Error: Missing arguments for compression." << RESET << std::endl;
            printUsage(argv[0]);
            return 1;
        }

        std::string outputFile = argv[3];
        if (outputFile.substr(outputFile.find_last_of(".") + 1) != "perzip")
        {
            std::cerr << RED << ERROR_EMOJI << " Error: Output file must have '.perzip' extension." << RESET << std::endl;
            printUsage(argv[0]);
            return 1;
        }

        if (!std::filesystem::exists(argv[2]))
        {
            std::cerr << RED << ERROR_EMOJI << " Error: Input file or directory '" << argv[2] << "' does not exist." << RESET << std::endl;
            return 1;
        }

        std::vector<std::string> allFiles = FileManager::getAllFilestoProcess(argv[2]);
        if (allFiles.empty())
        {
            std::cerr << RED << ERROR_EMOJI << " Error: No files found to process." << RESET << std::endl;
            return 1;
        }

        json jsonData = compress(argv[2], allFiles, PRIME1, PRIME2);
        if (FileManager::saveJsonFile(argv[3], jsonData))
        {
            std::cout << GREEN << CHECK_EMOJI << " JSON file created successfully: " << argv[3] << RESET << std::endl;
        }
        else
        {
            std::cerr << RED << ERROR_EMOJI << " Error: Failed to create JSON file." << RESET << std::endl;
            return 1;
        }
    }
    else if (option == "--decompress" || option == "-d")
    {
        if (argc < 4)
        {
            std::cerr << RED << ERROR_EMOJI << " Error: Missing arguments for decompression." << RESET << std::endl;
            printUsage(argv[0]);
            return 1;
        }

        if (!std::filesystem::is_directory(argv[3]))
        {
            std::cerr << RED << ERROR_EMOJI << " Error: Output path '" << argv[3] << "' must be a directory." << RESET << std::endl;
            return 1;
        }
        std::string inputFile = argv[2];
        std::string outputFile = argv[3];
        std::string regexStr = argc > 4 ? argv[4] : "";
        decompress(inputFile.c_str(), outputFile.c_str(), regexStr, PRIME1, PRIME2);
    }
    else if (option == "--show" || option == "-s")
    {
        if (argc < 3)
        {
            std::cerr << RED << ERROR_EMOJI << " Error: Missing input file for --show option." << RESET << std::endl;
            printUsage(argv[0]);
            return 1;
        }
        std::string inputFile = argv[2];
        ArchiveData archive = FileManager::loadJsonFile(inputFile);

        std::vector<std::string> file_names;
        for (size_t i = 0; i < archive.files.size(); i++)
        {
            file_names.push_back(archive.files[i].file_name);
        }
        std::sort(file_names.begin(), file_names.end());

        std::cout << BLUE << "\n"
                  << FOLDER_EMOJI << " Files in " << inputFile << ":" << RESET << "\n";
        if (file_names.empty())
        {
            std::cout << YELLOW << "  No files found." << RESET << std::endl;
        }
        else
        {
            for (const auto &file_name : file_names)
            {
                std::cout << GREEN << "  - " << FILE_EMOJI << " " << file_name << RESET << std::endl;
            }
        }
    }
    else
    {
        std::cerr << RED << ERROR_EMOJI << " Error: Unknown option '" << option << "'." << RESET << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    return 0;
}