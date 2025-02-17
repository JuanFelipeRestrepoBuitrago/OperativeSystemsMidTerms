#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()
#include <string>
#include <vector>

class FileManager {
public:
    static std::vector<char> readFile(const std::string& filePath);
    static std::vector<char> readBits(const std::string& filePath);
    static void writeFile(const std::string& filePath, const std::vector<char>& data);
    static void writeBits(const std::string& filePath, const std::vector<char>& data);

};

#endif
