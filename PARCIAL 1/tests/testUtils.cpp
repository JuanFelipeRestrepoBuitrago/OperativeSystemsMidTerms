#include <gtest/gtest.h>
#include "../Utils.h"

using std::vector;

TEST(UtilsTest, StringToCAndCToString) {
    const char* expectedStr = "Hola Soy Pipe";
    vector<int> bytes = Utils::stringToC(expectedStr);
    char* str = Utils::cToString(bytes);

    EXPECT_STREQ(expectedStr, str);
    Utils::freeCString(str);

    // Test 2
    const char* expectedStr2 = "Hola Soy Pipe!";
    const vector<int> expected = vector<int>({5, 7990271});
    char* str2 = Utils::cToString(expected);
    size_t dataLength = strlen(str2);
    // Debug: Print raw bytes of str2
    std::cout << "str2 bytes (hex): ";
    for (size_t i = 0; i < dataLength; i++) {
        std::cout << std::hex << static_cast<int>(static_cast<unsigned char>(str2[i])) << " ";
    }
    std::cout << std::dec << std::endl;
    vector<int> bytes2 = Utils::stringToC(str2);

    EXPECT_EQ(expected, bytes2);
    Utils::freeCString(str2);
}

TEST(UtilsTest, PowerModulus) {
    EXPECT_EQ(Utils::powerModulus(72, 5, 7990271), 1272050);
    EXPECT_EQ(Utils::powerModulus(108, 5, 7990271), 7162670);
    EXPECT_EQ(Utils::powerModulus(1272050, 1596269, 7990271), 72);
}

TEST(UtilsTest, ModInverse) {
    EXPECT_EQ(Utils::modInverse(5, 12), 5);
    EXPECT_EQ(Utils::modInverse(7, 40), 23);
    EXPECT_EQ(Utils::modInverse(5, 12), 5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}