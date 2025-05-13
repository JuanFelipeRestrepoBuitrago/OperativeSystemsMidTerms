#include "FileManager.h"

namespace fs = std::filesystem;

void FileManager::writeBinaryFile(const std::string& filePath, const std::vector<char>& data) {
    /**
     * Function to write compressed bit data to a file
     * 
     * @param filePath: The path of the file where the bit data will be written
     * @param data: The compressed data to be written
     * 
     * @return: None
     */
    int fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    std::vector<uint8_t> byteData;
    uint8_t currentByte = 0;
    int bitCount = 0;

    uint8_t validBits = data.size() % 8 == 0 ? 8 : data.size() % 8;
    byteData.push_back(validBits);

    for (char bit : data) {
        if (bit == '1') currentByte = (currentByte << 1) | 1;
        else currentByte = (currentByte << 1);

        bitCount++;

        if (bitCount == 8) {
            byteData.push_back(currentByte);
            currentByte = 0;
            bitCount = 0;
        }
    }
    
    if (bitCount > 0) {
        currentByte <<= (8 - bitCount);
        byteData.push_back(currentByte);
    }

    write(fd, byteData.data(), byteData.size());
    close(fd);
}

std::vector<uint8_t> FileManager::readBinaryFile(const std::string& path) {
    std::ifstream in{path, std::ios::binary | std::ios::ate};
    if (!in) {
        throw std::runtime_error("Cannot open file: " + path);
    }

    // determine size and rewind
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);

    // read all bytes
    std::vector<uint8_t> buffer(size);
    if (!in.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + path);
    }

    return buffer;
}

std::vector<std::string> FileManager::getAllFilestoProcess(const std::string& path) {
    /**
     * Function to get all files in a directory and its subdirectories
     * 
     * @param path: The path of the directory to search for files
     * 
     * @return: A vector containing the paths of all files found
     */
    std::vector<std::string> files;

    // Check if the path is a directory
    if (!fs::is_directory(path)) {
        files.push_back(path); 
        return files;
    }

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_regular_file(entry)) {
            files.push_back(entry.path().string()); 
        }
    }

    return files;
}

bool FileManager::saveJsonFile(const std::string& filePath, const json& jsonData) {
    /**
     * Function to save JSON data to a file
     * 
     * @param filePath: The path of the file where the JSON data will be saved
     * @param jsonData: The JSON data to be saved
     * 
     * @return: None
     */
    std::ofstream file(filePath);
    if (!file) {
        std::cerr << "Failed to open output JSON file." << std::endl;
        return false;
    }
    else if (file.is_open()) {
        file << jsonData.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        return true;
    } else {
        std::cerr << "Error opening file for writing JSON data." << std::endl;
        return false;
    }
}

ArchiveData FileManager::loadJsonFile(const std::string& filePath) {
    /**
     * Function to load JSON data from a file
     * 
     * @param filePath: The path of the file from which the JSON data will be loaded
     * 
     * @return: A struct containing the loaded JSON data
     */
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open input JSON file.");
    }

    json jsonData;
    file >> jsonData;

    ArchiveData archive;

    // Validate JSON structure
    if (!jsonData.is_object()) {
        throw std::runtime_error("Invalid JSON format: root must be an object");
    }

    // Extract public_key
    if (!jsonData.contains("public_key") || !jsonData["public_key"].is_string()) {
        throw std::runtime_error("Invalid JSON: missing or invalid 'public_key'");
    }
    archive.public_key = jsonData["public_key"].get<std::string>();

    // Extract private_key
    if (!jsonData.contains("private_key") || !jsonData["private_key"].is_string()) {
        throw std::runtime_error("Invalid JSON: missing or invalid 'private_key'");
    }
    archive.private_key = jsonData["private_key"].get<std::string>();

    // Extract files
    if (!jsonData.contains("files") || !jsonData["files"].is_array()) {
        throw std::runtime_error("Invalid JSON: missing or invalid 'files' array");
    }

    for (const auto& fileEntryJson : jsonData["files"]) {
        if (!fileEntryJson.is_object()) {
            std::cerr << "Warning: Skipping invalid file entry (not an object)" << std::endl;
            continue;
        }

        FileEntry fileEntry;

        if (!fileEntryJson.contains("file_name") || !fileEntryJson["file_name"].is_string()) {
            std::cerr << "Warning: Missing or invalid 'file_name' in file entry" << std::endl;
            continue;
        }
        fileEntry.file_name = fileEntryJson["file_name"].get<std::string>();

        if (!fileEntryJson.contains("file_data") || !fileEntryJson["file_data"].is_string()) {
            std::cerr << "Warning: Missing or invalid 'file_data' in file entry" << std::endl;
            continue;
        }
        fileEntry.file_data = fileEntryJson["file_data"].get<std::string>();

        if (!fileEntryJson.contains("huffman_table") || !fileEntryJson["huffman_table"].is_array()) {
            std::cerr << "Warning: Missing or invalid 'huffman_table' in file entry" << std::endl;
            continue;
        }
        for (const auto& tableEntry : fileEntryJson["huffman_table"]) {
            if (!tableEntry.is_object() ||
                !tableEntry.contains("letter") || !(tableEntry["letter"].is_number_unsigned()
                    || tableEntry["letter"].is_number_integer()) ||
                !tableEntry.contains("code") || !tableEntry["code"].is_string()) {
                std::cerr << "Warning: Skipping invalid Huffman table entry in " << fileEntry.file_name << std::endl;
                continue;
            }
            int letterVal = tableEntry["letter"].get<int>();
            char letter = static_cast<char>(letterVal);
            std::string code = tableEntry["code"].get<std::string>();
            fileEntry.huffman_table[code] = letter;
        }
        archive.files.push_back(fileEntry);
    }

    if (archive.files.empty()) {
        throw std::runtime_error("Invalid JSON: no valid files found");
    }

    // Close and clean up
    file.close();
    return archive;
}