#include "FileManager.h"

namespace fs = std::filesystem;

std::vector<char> FileManager::readTextFile(const std::string& filePath) {
    /**
     * Function to read the contents of a file
     * 
     * @param filePath: The path of the file to be read
     * 
     * @return: A vector containing the file contents as characters
     */
    int fd = open(filePath.c_str(), O_RDONLY); // Open the file in read-only mode
    if (fd == -1) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    std::vector<char> buffer;
    char chunk[4096]; // Temporary buffer to read the file in 4096-byte chunks
    ssize_t bytesRead;

    while ((bytesRead = read(fd, chunk, sizeof(chunk))) > 0) {
        buffer.insert(buffer.end(), chunk, chunk + bytesRead);
    }

    if (bytesRead == -1) {
        perror("Error reading from file");
    }

    close(fd); // Close the file after reading
    return buffer;
}

void FileManager::writeTextFile(const std::string& filePath, const std::vector<char>& data) {
    /**
     * Function to write data to a file
     * 
     * @param filePath: The path of the file to be written to
     * @param data: The data to be written to the file
     * 
     * @return: None
     */
    int fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open the file for writing, create if not exists, truncate if exists
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesWritten = write(fd, data.data(), data.size());

    if (bytesWritten == -1) {
        perror("Error writing to file");
    }

    close(fd);
}

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

std::vector<char> FileManager::readBinaryFile(const std::string& filePath) {
    /**
     * Function to read compressed bit data from a file
     * 
     * @param filePath: The path of the file from which the bit data will be read
     * 
     * @return: A vector containing the decompressed bit data
     */
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    uint8_t validBits;
    if (read(fd, &validBits, 1) != 1) {
        perror("Error reading validBits");
        close(fd);
        exit(EXIT_FAILURE);
    }

    std::vector<uint8_t> byteData;
    uint8_t byte;
    while (read(fd, &byte, 1)) {
        byteData.push_back(byte);
    }
    close(fd);

    std::vector<char> data;
    for (size_t i = 0; i < byteData.size(); ++i) {
        uint8_t currentByte = byteData[i];
        int bitsToRead = (i == byteData.size() - 1) ? validBits : 8;
        
        for (int j = bitsToRead - 1; j >= 0; --j) {
            char bit = ((currentByte >> j) & 1) ? '1' : '0';
            data.push_back(bit);
        }
    }

    return data;
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