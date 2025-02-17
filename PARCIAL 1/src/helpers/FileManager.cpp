#include "FileManager.h"
#include <iostream>

std::vector<char> FileManager::readFile(const std::string& filePath) {
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

void FileManager::writeFile(const std::string& filePath, const std::vector<char>& data) {
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

void FileManager::writeBits(const std::string& filePath, const std::vector<char>& data) {
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

std::vector<char> FileManager::readBits(const std::string& filePath) {
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