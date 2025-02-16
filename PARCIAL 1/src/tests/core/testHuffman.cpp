#include <gtest/gtest.h>
#include "../../core/Huffman.h"
#include "../../helpers/FileManager.h"

#define TEMPLATE_PATH "src/tests/messages/templateHuffman.txt"

void printMessage(const std::vector<char>& compressedMessage) {
    for (char c : compressedMessage) {
        std::cout << c;
    }
    std::cout << std::endl;
}

TEST(HuffmanTest, CompressAndUncompress) {
    Huffman huffman;
    std::unordered_map<char, int> freqMap = {
        {'a', 3},
        {'b', 4},
        {'c', 2},
        {'d', 1},
    };
    huffman.buildTree(freqMap);

    std::vector<char> message = FileManager::readFile(TEMPLATE_PATH);

    std::vector<char> compressedMessage = huffman.compress(message);
    std::vector<char> uncompressedMessage = huffman.uncompress(compressedMessage);

    printMessage(message);
    printMessage(compressedMessage);
    printMessage(uncompressedMessage);

    EXPECT_EQ(message, uncompressedMessage);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}