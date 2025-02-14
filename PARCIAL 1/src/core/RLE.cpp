#include "RLE.h"

RLE::RLE(const std::string &path) : filePath(path) {}

std::vector<char> RLE::compress(const std::vector<char> &data) {
    std::vector<char> compressedMessage;
    size_t i = 0;

    while (i < data.size()) {
        char currentChar = data[i];
        int count = 1;

        while (i + 1 < data.size() && data[i] == data[i + 1]) {
            count++;
            i++;
        }

        std::string countStr = std::to_string(count);
        compressedMessage.insert(compressedMessage.end(), countStr.begin(), countStr.end());

        compressedMessage.push_back(currentChar);

        i++;
    }

    return compressedMessage;
}

std::vector<char> RLE::uncompress(const std::vector<char> &data) {
    std::vector<char> uncompressedMessage;
    size_t i = 0;

    while (i < data.size()) {
        int count = 0;
        while (std::isdigit(data[i])) {
            count = count * 10 + data[i] - '0';
            i++;
        }

        char currentChar = data[i];
        uncompressedMessage.insert(uncompressedMessage.end(), count, currentChar);

        i++;
    }

    return uncompressedMessage;
}
