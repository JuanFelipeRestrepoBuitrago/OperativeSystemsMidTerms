#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()
#include <string>
#include <vector>
#include <cstdint> 
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../libs/json.hpp"

using json = nlohmann::json;

class FileManager {
public:
    static std::vector<uint8_t> readBinaryFile(const std::string& filePath);
    static void writeBinaryFile(const std::string& filePath, const std::vector<char>& data);
    static std::vector<std::string> getAllFilestoProcess(const std::string& path);
    static bool saveJsonFile(const std::string& filePath, const json& jsonData);
};

#endif
