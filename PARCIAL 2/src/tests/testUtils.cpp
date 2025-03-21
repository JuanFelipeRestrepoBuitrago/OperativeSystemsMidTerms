#include <gtest/gtest.h>
#include "../utils/Utils.h"

TEST(UtilsTest, ImageRotationSizeCalculation) {
    int width, height;

    // Test square image (100x100)
    width = 100, height = 100;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 142);
    EXPECT_EQ(height, 142);

    // Test small square (4x4)
    width = 4, height = 4;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 6);
    EXPECT_EQ(height, 6);

    // Test rectangular image (4x3)
    width = 4, height = 3;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 5);
    EXPECT_EQ(height, 5);

    // Test larger rectangular (6x8)
    width = 6, height = 8;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 10);
    EXPECT_EQ(height, 10);

    // Test odd dimensions (5x5)
    width = 5, height = 5;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 6);
    EXPECT_EQ(height, 6);

    // Test non-square odd (7x3)
    width = 7, height = 3;
    Utils::getSizeImageRotated(width, height);
    EXPECT_EQ(width, 7);
    EXPECT_EQ(height, 7);
}

TEST(UtilsTest, ImageScalingSizeCalculation) {
    int width, height;

    // Scale down by 0.5 (200x100 → 100x50)
    width = 200, height = 100;
    Utils::getSizeImageScaled(width, height, 0.5);
    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 50);
    EXPECT_EQ(width * height, 5000); // Verify total size

    // Scale down with rounding (100x50 → 25x13)
    width = 100, height = 50;
    Utils::getSizeImageScaled(width, height, 0.25);
    EXPECT_EQ(width, 25);
    EXPECT_EQ(height, 13);
    EXPECT_EQ(width * height, 325);

    // Scale up (10x10 → 22x22)
    width = 10, height = 10;
    Utils::getSizeImageScaled(width, height, 2.2);
    EXPECT_EQ(width, 22);
    EXPECT_EQ(height, 22);
    EXPECT_EQ(width * height, 484);

    // Integer scaling (15x20 → 45x60)
    width = 15, height = 20;
    Utils::getSizeImageScaled(width, height, 3.0);
    EXPECT_EQ(width, 45);
    EXPECT_EQ(height, 60);
    EXPECT_EQ(width * height, 2700);

    // No scaling (30x40 remains)
    width = 30, height = 40;
    Utils::getSizeImageScaled(width, height, 1.0);
    EXPECT_EQ(width, 30);
    EXPECT_EQ(height, 40);
    EXPECT_EQ(width * height, 1200);
}

TEST(UtilsTest, PowerOfTwoRounding) {
    // Exact powers of two
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(1), 1);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(16), 16);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(1024), 1024);

    // Round up to next power
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(3), 4);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(15), 16);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(100), 128);

    // Edge cases
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(0), 1); // Handle 0 input
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(1), 1);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(2), 2);
}

TEST(UtilsTest, CombinedOperations) {
    // Rotate then round to power-of-two
    int w = 4, h = 3;
    Utils::getSizeImageRotated(w, h);
    size_t rotated_size = w * h; // 5x5 = 25
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(rotated_size), 32);

    // Scale then round to power-of-two
    w = 15, h = 20;
    Utils::getSizeImageScaled(w, h, 3.0);
    size_t scaled_size = w * h; // 45x60 = 2700
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(scaled_size), 4096);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}