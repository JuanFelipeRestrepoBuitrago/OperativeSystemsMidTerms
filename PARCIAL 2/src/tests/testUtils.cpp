#include <gtest/gtest.h>
#include "../utils/Utils.h"

TEST(UtilsTest, ImageRotationSizeCalculation) {
    int width, height;

    // Test 45° rotation for square image
    width = 100, height = 100;
    Utils::getSizeImageRotated(width, height, 45.0);
    EXPECT_EQ(width, 142);  // 100*(√2/2) + 100*(√2/2) = 141.42 → ceil
    EXPECT_EQ(height, 142);

    // Test 30° rotation for rectangular image
    width = 4, height = 3;
    Utils::getSizeImageRotated(width, height, 30.0);
    EXPECT_EQ(width, 5);  // 4*cos(30°) + 3*sin(30°) = 4*0.866 + 3*0.5 = 4.964 → 5
    EXPECT_EQ(height, 5); // 4*sin(30°) + 3*cos(30°) = 4*0.5 + 3*0.866 = 4.598 → 5

    // Test 60° rotation for odd dimensions
    width = 5, height = 5;
    Utils::getSizeImageRotated(width, height, 60.0);
    EXPECT_EQ(width, 7);  // 5*cos(60°) + 5*sin(60°) = 5*0.5 + 5*0.866 = 6.83 → 7
    EXPECT_EQ(height, 7);
}

TEST(UtilsTest, ImageScalingSizeCalculation) {
    int width, height;

    // Test exact scaling
    width = 200, height = 100;
    Utils::getSizeImageScaled(width, height, 0.5);
    EXPECT_EQ(width, 100);  // 200*0.5 = 100 (exact)
    EXPECT_EQ(height, 50);

    // Test fractional scaling with ceil
    width = 99, height = 50;
    Utils::getSizeImageScaled(width, height, 0.3);
    EXPECT_EQ(width, 30);  // 99*0.3 = 29.7 → 30
    EXPECT_EQ(height, 15); // 50*0.3 = 15 (exact)

    // Test minimum size constraint
    width = 2, height = 2;
    Utils::getSizeImageScaled(width, height, 0.1);
    EXPECT_EQ(width, 1);  // 2*0.1 = 0.2 → ceil to 1
    EXPECT_EQ(height, 1);
}

TEST(UtilsTest, PowerOfTwoRounding) {
    // Test near powers of two
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(1023), 1024);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(1025), 2048);

    // Test exact power with large number
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(131072), 131072);
}

TEST(UtilsTest, CombinedOperations) {
    // Rotate then scale
    int w = 400, h = 300;
    Utils::getSizeImageRotated(w, h, 45.0);
    Utils::getSizeImageScaled(w, h, 0.75);
    EXPECT_EQ(w, 372); 
    EXPECT_EQ(h, 372);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}