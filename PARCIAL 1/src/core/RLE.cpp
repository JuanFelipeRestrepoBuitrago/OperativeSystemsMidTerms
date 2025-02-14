#include "RLE.h"

RLE::RLE(const std::string &path) : filePath(path) {}

std::vector<char> RLE::compress(const std::vector<char> &data) {
    std::vector<char> encoded;
    size_t i = 0;

    while (i < data.size()) {
        char currentChar = data[i];
        int count = 1;

        while (i + 1 < data.size() && data[i] == data[i + 1]) {
            count++;
            i++;
        }

        std::string countStr = std::to_string(count);
        encoded.insert(encoded.end(), countStr.begin(), countStr.end());

        encoded.push_back(currentChar);

        i++;
    }

    return encoded;
}
