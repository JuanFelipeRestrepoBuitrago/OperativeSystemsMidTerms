#include <gtest/gtest.h>
#include "../utils/Utils.h"

TEST(UtilsTest, ImageRotationSizeCalculation) {
    // Test square image
    EXPECT_EQ(Utils::getSizeImageRotated(100, 100), 19881); // 141x141
    EXPECT_EQ(Utils::getSizeImageRotated(4, 4), 25);        // 5x5
    
    // Test rectangular image
    EXPECT_EQ(Utils::getSizeImageRotated(4, 3), 25);        // 5x5
    EXPECT_EQ(Utils::getSizeImageRotated(6, 8), 100);       // 10x10
    
    // Test odd dimensions
    EXPECT_EQ(Utils::getSizeImageRotated(5, 5), 49);        // 7x7
    EXPECT_EQ(Utils::getSizeImageRotated(7, 3), 25);        // 5x5
}

TEST(UtilsTest, ImageScalingSizeCalculation) {
    // Test scale down
    EXPECT_EQ(Utils::getSizeImageScaled(200, 100, 0.5), 5000);   // 100x50
    EXPECT_EQ(Utils::getSizeImageScaled(100, 50, 0.25), 312);    // 25x12.5 -> 26x13 = 338?
    
    // Test scale up
    EXPECT_EQ(Utils::getSizeImageScaled(10, 10, 2.2), 484);      // 22x22
    EXPECT_EQ(Utils::getSizeImageScaled(15, 20, 3.0), 2700);     // 45x60
    
    // Test same size
    EXPECT_EQ(Utils::getSizeImageScaled(30, 40, 1.0), 1200);     // 30x40
}

TEST(UtilsTest, PowerOfTwoRounding) {
    // Exact powers
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(1), 1);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(16), 16);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(1024), 1024);
    
    // Between powers
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(3), 4);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(15), 16);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(100), 128);
    
    // Edge cases
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(0), 1);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(1), 1);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(2), 2);
}

TEST(UtilsTest, CombinedOperations) {
    // Test rotation followed by power-of-two rounding
    size_t rotated_size = Utils::getSizeImageRotated(4, 3);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(rotated_size), 32);  // 25 -> 32
    
    // Test scaling followed by power-of-two rounding
    size_t scaled_size = Utils::getSizeImageScaled(15, 20, 3.0);
    EXPECT_EQ(Utils::roundToNextPowerOfTwo(scaled_size), 4096); // 2700 -> 4096
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}