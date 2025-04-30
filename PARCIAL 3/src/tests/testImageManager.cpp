#include <gtest/gtest.h>
#include "../core/ImageManager.h"

class ImageManagerTest : public ::testing::Test {
protected:
    int width = 4;
    int height = 4;
    int channels = 3;
    ImageManager* imgManager;
    unsigned char** originalPixels;
    unsigned char** transformedPixels;

    void SetUp() override {
        imgManager = new ImageManager(width, height, channels);

        originalPixels = new unsigned char*[height];
        for(int y = 0; y < height; y++) {
            originalPixels[y] = new unsigned char[width * channels];
            for(int x = 0; x < width; x++) {
                int idx = x * channels;
                originalPixels[y][idx] = 10;
                originalPixels[y][idx + 1] = 20;
                originalPixels[y][idx + 2] = 30;
            }
        }

        transformedPixels = new unsigned char*[height];
        for(int y = 0; y < height; y++) {
            transformedPixels[y] = new unsigned char[width * channels];
        }
    }

    void TearDown() override {
        for(int y = 0; y < height; y++) {
            delete[] originalPixels[y];
            delete[] transformedPixels[y];
        }
        delete[] originalPixels;
        delete[] transformedPixels;
        delete imgManager;
    }
};

TEST_F(ImageManagerTest, ConstructorInitializesCorrectly) {
    EXPECT_EQ(imgManager->getWidth(), width);
    EXPECT_EQ(imgManager->getHeight(), height);
    EXPECT_EQ(imgManager->getChannels(), channels);
}

TEST_F(ImageManagerTest, InitializeBufferWithColor) {
    Pixel fillColor = {100, 150, 200, 255};
    imgManager->initializeBufferWithColor(fillColor, transformedPixels, width, height);

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int idx = x * channels;
            EXPECT_EQ(transformedPixels[y][idx], fillColor.r);
            EXPECT_EQ(transformedPixels[y][idx + 1], fillColor.g);
            EXPECT_EQ(transformedPixels[y][idx + 2], fillColor.b);
        }
    }
}

TEST_F(ImageManagerTest, RotateImageKeepsDimensions) {
    int newWidth = 6;
    int newHeight = 6;
    Pixel fillColor = {0, 0, 0, 255};

    unsigned char** rotatedPixels = new unsigned char*[newHeight];
    for(int y = 0; y < newHeight; y++) {
        rotatedPixels[y] = new unsigned char[newWidth * channels];
    }

    imgManager->rotateImage(45.0f, fillColor, rotatedPixels, originalPixels, newWidth, newHeight);

    EXPECT_EQ(imgManager->getWidth(), newWidth);
    EXPECT_EQ(imgManager->getHeight(), newHeight);

    int cx = newWidth / 2;
    int cy = newHeight / 2;
    int idx = cx * channels;
    EXPECT_EQ(rotatedPixels[cy][idx], 10);
    EXPECT_EQ(rotatedPixels[cy][idx + 1], 20);
    EXPECT_EQ(rotatedPixels[cy][idx + 2], 30);

    for(int y = 0; y < newHeight; y++) {
        delete[] rotatedPixels[y];
    }
    delete[] rotatedPixels;
}

TEST_F(ImageManagerTest, ScaleImageChangesDimensionsCorrectly) {
    float scaleFactor = 2.0f;
    int newWidth = static_cast<int>(width * scaleFactor);
    int newHeight = static_cast<int>(height * scaleFactor);

    unsigned char** scaledPixels = new unsigned char*[newHeight];
    for (int y = 0; y < newHeight; y++) {
        scaledPixels[y] = new unsigned char[newWidth * channels];
    }

    imgManager->scaleImage(scaleFactor, scaledPixels, originalPixels);

    EXPECT_EQ(imgManager->getWidth(), newWidth);
    EXPECT_EQ(imgManager->getHeight(), newHeight);

    int cx = newWidth / 2;
    int cy = newHeight / 2;
    int idx = cx * channels;

    EXPECT_GE(scaledPixels[cy][idx], 0);
    EXPECT_LE(scaledPixels[cy][idx], 255);

    for (int y = 0; y < newHeight; y++) {
        delete[] scaledPixels[y];
    }
    delete[] scaledPixels;
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}