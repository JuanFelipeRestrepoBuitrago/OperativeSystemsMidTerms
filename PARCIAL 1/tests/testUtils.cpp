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
    const char* expectedStr2 = "5 7990271";
    vector<int> bytes2 = Utils::stringToC(expectedStr2);
    char* str2 = Utils::cToString(bytes2);

    EXPECT_STREQ(expectedStr2, str2);
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