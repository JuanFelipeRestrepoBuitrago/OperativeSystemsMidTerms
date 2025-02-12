#include "../Utils.h"
#include "../RSA.h"
#include <gtest/gtest.h>

using std::vector;
using std::string;
using std::cout;
using std::endl;

TEST(RSATest, GenerateKeys) {
    Rsa rsa(7919, 1009);
    ResultGenerateKeys keys = rsa.generateKeys();

    vector<int> publicKeyValues = Utils::base64ToNumbers(keys.publicKey);
    vector<int> privateKeyValues = Utils::base64ToNumbers(keys.privateKey);
    std::cout << "Public Key (e, n): (" << publicKeyValues[0] << ", " << publicKeyValues[1] << ")\n";
    std::cout << "Private Key (d, n): (" << privateKeyValues[0] << ", " << privateKeyValues[1] << ")\n";
    std::cout << "Public Key: " << keys.publicKey << std::endl;
    std::cout << "Private Key: " << keys.privateKey << std::endl;

    EXPECT_EQ(publicKeyValues.size(), 2);
    EXPECT_EQ(privateKeyValues.size(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}