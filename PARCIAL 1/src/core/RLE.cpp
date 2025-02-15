#include "RLE.h"

/**
 * Constructor for the RLE class
 *
 * @param path: The path to the file to be compressed or decompressed
 * @return: None
 */
RLE::RLE(const std::string &path) : filePath(path) {}

std::vector<char> RLE::compress(const std::vector<char> &data) {
    /**
     * Function to compress a given input using RLE.
     *
     * This method encodes repeated characters by replacing them with a count 
     * followed by the character itself.
     *
     * @param data: The input vector of characters to be compressed
     * @return: A vector of compressed characters
     *
     * @example
     * Input:  {'A', 'A', 'A', 'B', 'B', 'C'}
     * Output: {'3', 'A', '2', 'B', '1', 'C'}
     */
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
    /**
     * Function to decompress an RLE-compressed input back to its original form.
     *
     * This method reconstructs the original data from the RLE format.
     *
     * @param data: The compressed input vector of characters
     * @return: A vector of uncompressed characters
     *
     * @example
     * Input:  {'3', 'A', '2', 'B', '1', 'C'}
     * Output: {'A', 'A', 'A', 'B', 'B', 'C'}
     */

    std::vector<char> uncompressedMessage;
    size_t i = 0;

    while (i < data.size()-1) {
        int count = data[i] - '0';
        char currentChar = data[i+1];
        uncompressedMessage.insert(uncompressedMessage.end(), count, currentChar);

        i+=2;
    }

    return uncompressedMessage;
}
