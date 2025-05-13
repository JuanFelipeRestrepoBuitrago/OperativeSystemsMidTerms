#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()
#include <string>
#include <vector>
#include <cstdint> 
#include <iostream>
#include <filesystem>

class FileManager {
public:
    static std::vector<char> readTextFile(const std::string& filePath);
    static std::vector<char> readBinaryFile(const std::string& filePath);
    static void writeTextFile(const std::string& filePath, const std::vector<char>& data);
    static void writeBinaryFile(const std::string& filePath, const std::vector<char>& data);
    static std::vector<std::string> getAllFilestoProcess(const std::string& path);
};

#endif
