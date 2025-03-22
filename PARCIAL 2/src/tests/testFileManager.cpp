#include <gtest/gtest.h>
#include <filesystem>
#include <chrono>
#include "../memory/FileManager.h"

namespace fs = std::filesystem;

class FileManagerTest : public ::testing::Test {
protected:
    const std::string test_input = "test_image.png";
    const std::string test_output = "output_image.png";
    const std::string non_existent_file = "non_existent.png";
    
    void SetUp() override {
        // Create a simple 2x2 test image
        const int w = 2, h = 2;
        unsigned char pixels[] = {
            255, 0,    // White, Black
            0,   255   // Black, White
        };
        if (!stbi_write_png(test_input.c_str(), w, h, 1, pixels, w)) {
            FAIL() << "Failed to create test image file.";
        }
    }

    void TearDown() override {
        if(fs::exists(test_input)) fs::remove(test_input);
        if(fs::exists(test_output)) fs::remove(test_output);
    }

    bool file_exists(const std::string& path) {
        return fs::exists(path);
    }
};

TEST_F(FileManagerTest, ReadValidFileWithAllocator) {
    FileManager fm(test_input, test_output, 
                  TransformationMethod::ROTATION, true);
    
    unsigned char** pixels = fm.initializeOriginalPixelsFromFile();
    ASSERT_NE(pixels, nullptr);
    
    // Verify pixel values
    EXPECT_EQ(pixels[0][0], 255);
    EXPECT_EQ(pixels[0][1], 0);
    EXPECT_EQ(pixels[1][0], 0);
    EXPECT_EQ(pixels[1][1], 255);
}

TEST_F(FileManagerTest, ReadNonExistentFile) {
    FileManager fm(non_existent_file, test_output,
                  TransformationMethod::ROTATION, false, nullptr);
    
    EXPECT_EXIT(fm.initializeOriginalPixelsFromFile(), 
               ::testing::ExitedWithCode(1), 
               "Error: Could not read image metadata.");
}

TEST_F(FileManagerTest, RotationTransformation) {
    FileManager fm(test_input, test_output,
                  TransformationMethod::ROTATION, false);
    
    // Original dimensions
    fm.getFileMetadata();
    EXPECT_EQ(fm.getWidth(), 2);
    EXPECT_EQ(fm.getHeight(), 2);

    // Get transformed pixels
    unsigned char** transformed = fm.initializeTransformedPixels();
    ASSERT_NE(transformed, nullptr);
    
    // Verify new dimensions (calculated by rotation logic)
    EXPECT_GT(fm.getTransformedImageWidth(), 2);
    EXPECT_GT(fm.getTransformedImageHeight(), 2);
}

TEST_F(FileManagerTest, ScalingTransformation) {
    double scale = 2.0;
    FileManager fm(test_input, test_output,
                  TransformationMethod::SCALING, false, &scale);
    
    fm.getFileMetadata();
    unsigned char** transformed = fm.initializeTransformedPixels();
    
    // Add assertion to use the variable
    ASSERT_NE(transformed, nullptr);
    EXPECT_EQ(fm.getTransformedImageWidth(), 4);
    EXPECT_EQ(fm.getTransformedImageHeight(), 4);
}

TEST_F(FileManagerTest, SaveTransformedImage) {
    FileManager fm(test_input, test_output,
                  TransformationMethod::SCALING, false);
    
    // Create dummy transformed data
    int new_w = 4, new_h = 4;
    unsigned char** data = new unsigned char*[new_h];
    for(int i = 0; i < new_h; i++) {
        data[i] = new unsigned char[new_w];
        memset(data[i], 128, new_w); // Gray color
    }
    
    EXPECT_NO_THROW(fm.saveImage(data, new_w, new_h, 1));
    EXPECT_TRUE(file_exists(test_output));

    // Cleanup
    for(int i = 0; i < new_h; i++) delete[] data[i];
    delete[] data;
}

TEST_F(FileManagerTest, BuddyAllocatorUsage) {
    FileManager fm(test_input, test_output,
                  TransformationMethod::ROTATION, true);

    // Original image allocation
    unsigned char** orig = fm.initializeOriginalPixelsFromFile();
    ASSERT_NE(orig, nullptr);
    
    // Transformed image allocation
    unsigned char** transformed = fm.initializeTransformedPixels();
    ASSERT_NE(transformed, nullptr);

    // Verify allocators were used
    EXPECT_NE(fm.getAllocatorOriginalImage(), nullptr);
    EXPECT_NE(fm.getAllocatorTransformedImage(), nullptr);
}

TEST_F(FileManagerTest, MemoryDeallocation) {
    double scale = 1.0;  // Or any valid scaling factor
    {
        FileManager fm(test_input, test_output,
                      TransformationMethod::SCALING, true, &scale);
        fm.initializeOriginalPixelsFromFile();
        fm.initializeTransformedPixels();
    } // Destructor called here

    SUCCEED();
}

TEST_F(FileManagerTest, MemoryAccessWithBuddyAllocator) {
    // With BuddyAllocator
    FileManager fm(test_input, test_output, TransformationMethod::ROTATION, true);
    unsigned char** pixels = fm.initializeOriginalPixelsFromFile();
    ASSERT_NE(pixels, nullptr);

    // Time memory access
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile unsigned char dummy = 0;  // Prevent compiler optimization
    for(int i = 0; i < fm.getHeight(); i++) {
        for(int j = 0; j < fm.getWidth(); j++) {
            dummy += pixels[i][j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "\nBuddyAllocator access time: " << duration.count() << " ns\n";
}

TEST_F(FileManagerTest, MemoryAccessWithoutBuddyAllocator) {
    // Without BuddyAllocator
    FileManager fm(test_input, test_output, TransformationMethod::ROTATION, false);
    unsigned char** pixels = fm.initializeOriginalPixelsFromFile();
    ASSERT_NE(pixels, nullptr);

    // Time memory access
    auto start = std::chrono::high_resolution_clock::now();

    volatile unsigned char dummy = 0;  // Prevent compiler optimization
    for(int i = 0; i < fm.getHeight(); i++) {
        for(int j = 0; j < fm.getWidth(); j++) {
            dummy += pixels[i][j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Standard allocation access time: " << duration.count() << " ns\n";
}

TEST_F(FileManagerTest, MemoryAccessTimeComparison) {
    // --- With Buddy Allocator ---
    FileManager fmBuddy("/home/pipertpo/D/ArchivosdeAplicaciones/VSCode/EAFIT/SEMESTRE 7/SISTEMAS OPERATIVOS/PARCIALES/PARCIAL 2/src/tests/images/test.jpg", test_output, TransformationMethod::ROTATION, true);
    fmBuddy.initializeOriginalPixelsFromFile();
    // Use volatile to prevent compiler optimizing out the loop
    volatile unsigned char sumBuddy = 0;
    
    auto startBuddy = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < fmBuddy.getHeight(); i++) {
        for (int j = 0; j < fmBuddy.getWidth(); j++) {
            sumBuddy += fmBuddy.getOriginalPixels()[i][j];
        }
    }
    auto endBuddy = std::chrono::high_resolution_clock::now();
    auto durationBuddy = std::chrono::duration_cast<std::chrono::microseconds>(endBuddy - startBuddy).count();

    // --- Without Buddy Allocator ---
    FileManager fmStd("/home/pipertpo/D/ArchivosdeAplicaciones/VSCode/EAFIT/SEMESTRE 7/SISTEMAS OPERATIVOS/PARCIALES/PARCIAL 2/src/tests/images/test.jpg", test_output, TransformationMethod::ROTATION, false);
    fmStd.initializeOriginalPixelsFromFile();
    volatile unsigned char sumStd = 0;
    
    auto startStd = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < fmStd.getHeight(); i++) {
        for (int j = 0; j < fmStd.getWidth(); j++) {
            sumStd += fmStd.getOriginalPixels()[i][j];
        }
    }
    auto endStd = std::chrono::high_resolution_clock::now();
    auto durationStd = std::chrono::duration_cast<std::chrono::microseconds>(endStd - startStd).count();

    // For debugging, output the measured times
    std::cout << "Buddy Allocator access time: " << durationBuddy << " µs" << std::endl;
    std::cout << "Standard allocation access time: " << durationStd << " µs" << std::endl;

    // For our test, we expect the buddy allocator version to be faster.
    // Note: This expectation might need some tolerance depending on your environment.
    EXPECT_LT(durationBuddy, durationStd);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}