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

    close(fd); // Close the file after writing
}
