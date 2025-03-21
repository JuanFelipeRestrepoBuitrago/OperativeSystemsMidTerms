#include <gtest/gtest.h>
#include <filesystem>
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
    
    EXPECT_NO_THROW(fm.saveImage(data));
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}