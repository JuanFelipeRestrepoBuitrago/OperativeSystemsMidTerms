#include <gtest/gtest.h>
#include "../utils/Utils.h"

TEST(UtilsTest, ImageRotationSizeCalculation) {
    int width, height;

    // Test 100x100 square image
    width = 100, height = 100;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 142);  // ceil(100*sqrt(2)) = ceil(141.42) → 142
    EXPECT_EQ(height, 142);

    // Test rectangular 4x3 image
    width = 4, height = 3;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 5);  // ceil(2*sqrt((2²)+(1.5²))) = ceil(5) → 5
    EXPECT_EQ(height, 5);

    // Test 5x5 odd dimensions
    width = 5, height = 5;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 8);  // ceil(5*sqrt(2)) = ceil(7.07) → 8
    EXPECT_EQ(height, 8);
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
    int w = 400, h = 300;
    Utils::getSizeImageRotated(w, h);
    Utils::getSizeImageScaled(w, h, 0.75);
    EXPECT_EQ(w, 375);
    EXPECT_EQ(h, 375);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}