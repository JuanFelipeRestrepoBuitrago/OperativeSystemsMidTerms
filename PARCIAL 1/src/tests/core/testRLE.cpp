#include <gtest/gtest.h>
#include "../../core/RLE.h"
#include "../../helpers/FileManager.h"

#define TEMPLATE_PATH "src/tests/messages/templateRLE.txt"

void printMessage(const std::vector<char>& compressedMessage) {
    for (char c : compressedMessage) {
        std::cout << c;
    }
    std::cout << std::endl;
}

TEST(RLETest, CompressAndUncompress) {
    RLE rle(TEMPLATE_PATH);

    std::vector<char> message = FileManager::readFile(TEMPLATE_PATH);

    std::vector<char> compressedMessage = rle.compress(message);
    std::vector<char> uncompressedMessage = rle.uncompress(compressedMessage);

    printMessage(message);
    printMessage(compressedMessage);
    printMessage(uncompressedMessage);

    EXPECT_EQ(message, uncompressedMessage);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}