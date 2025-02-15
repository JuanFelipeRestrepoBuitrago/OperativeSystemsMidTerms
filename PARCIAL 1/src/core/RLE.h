#ifndef RLE_H
#define RLE_H

#include <iostream>
#include <string>
#include <vector>

class RLE {
private:
    std::string filePath;

public:
    RLE(const std::string &path);
    std::vector<char> compress(const std::vector<char> &data);
    std::vector<char> uncompress(const std::vector<char> &data);
};

#endif
