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