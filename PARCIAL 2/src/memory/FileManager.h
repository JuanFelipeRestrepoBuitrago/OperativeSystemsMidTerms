#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()
#include <string>
#include "BuddyAllocator.h"
#include "../utils/Utils.h"
#include "../standardImage/stb_image.h"
#include "../standardImage/stb_image_write.h"
#include <iostream>
#include <fstream>
#include <cstring>


enum class TransformationMethod {
    ROTATION,   // Rotate the image
    SCALING     // Scale the image
};

class FileManager {
private:
    std::string readFilePath;
    std::string writeFilePath;
    TransformationMethod transformationMethod;
    int width;
    int height;
    int channels;
    int transformedImageWidth;
    int transformedImageHeight;
    BuddyAllocator* allocatorOriginalImage;
    BuddyAllocator* allocatorTransformedImage;
    unsigned char** originalPixels;
    unsigned char** transformedPixels;
    bool buddyAllocatorUsage;
    // If degrees, give the angle in degrees. If scaling, give the scaling factor
    double transformationFactor;

    unsigned char** allocateMemory(int width, int height, BuddyAllocator* allocator, bool parallelize);
    void deallocateMemory(unsigned char** pixels, int height);
public:
    FileManager(const std::string& readFilePath, const std::string& writeFilePath, TransformationMethod transformationMethod, bool buddyAllocatorUsage, double factor);
    ~FileManager();

    unsigned char** initializeOriginalPixelsFromFile(bool parallelize);
    unsigned char** initializeTransformedPixels(bool parallelize);
    void saveImage(unsigned char** data, int width, int height, int channels);
    void getFileMetadata();
    void showFileInfo() const;

    // Getters and setters
    std::string getReadFilePath() { return readFilePath; }
    std::string getWriteFilePath() { return writeFilePath; }
    TransformationMethod getTransformationMethod() { return transformationMethod; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getChannels() { return channels; }
    int getTransformedImageWidth() { return transformedImageWidth; }
    int getTransformedImageHeight() { return transformedImageHeight; }
    unsigned char** getOriginalPixels() { return originalPixels; }
    unsigned char** getTransformedPixels() { return transformedPixels; }
    BuddyAllocator* getAllocatorOriginalImage() { return allocatorOriginalImage; }
    BuddyAllocator* getAllocatorTransformedImage() { return allocatorTransformedImage; }
    bool getBuddyAllocatorUsage() { return buddyAllocatorUsage; }
    double getTransformationFactor() { return transformationFactor; }
    void setOriginalPixels(unsigned char** pixels) { originalPixels = pixels; }
    void setTransformedPixels(unsigned char** pixels) { transformedPixels = pixels; }
    void setAllocatorOriginalImage(BuddyAllocator* allocator) { allocatorOriginalImage = allocator; }
    void setAllocatorTransformedImage(BuddyAllocator* allocator) { allocatorTransformedImage = allocator; }
    void setBuddyAllocatorUsage(bool usage) { buddyAllocatorUsage = usage; }
    void setTransformationFactor(double factor) { transformationFactor = factor; }
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    void setChannels(int c) { channels = c; }
    void setTransformedImageWidth(int w) { transformedImageWidth = w; }
    void setTransformedImageHeight(int h) { transformedImageHeight = h; }
    void setReadFilePath(const std::string& path) { readFilePath = path; }
    void setWriteFilePath(const std::string& path) { writeFilePath = path; }
    void setTransformationMethod(TransformationMethod method) { transformationMethod = method; }
};

#endif
