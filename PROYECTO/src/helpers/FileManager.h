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
#include <unordered_map>
#include "../libs/json.hpp"

using json = nlohmann::json;

struct FileEntry {
    std::string file_name;
    std::string file_data;
    std::unordered_map<std::string, char> huffman_table;
};

struct ArchiveData {
    std::string public_key;
    std::string private_key;
    std::vector<FileEntry> files;
};

class FileManager {
public:
    static std::vector<uint8_t> readBinaryFile(const std::string& filePath);
    static void writeBinaryFile(const std::string& filePath, const std::vector<char>& data);
    static bool writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);
    static std::vector<std::string> getAllFilestoProcess(const std::string& path);
    static bool saveJsonFile(const std::string& filePath, const json& jsonData);
    static ArchiveData loadJsonFile(const std::string& filePath);
};

#endif
