#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()
#include <string>
#include "BuddyAllocator.h"

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
    BuddyAllocator* allocatorOriginalImage;
    BuddyAllocator* allocatorTransformedImage;
    unsigned char** originalPixels;
    unsigned char** transformedPixels;
    bool buddyAllocatorUsage;

    unsigned char** allocateMemory(int width, int height);
    void deallocateMemory(unsigned char** pixels, int height);
public:
    FileManager(const std::string& readFilePath, const std::string& writeFilePath, TransformationMethod transformationMethod, bool buddyAllocatorUsage);
    ~FileManager();

    unsigned char** getOriginalPixelsFromFile();
    unsigned char** getTransformedPixels();
    void saveImage(unsigned char*** data);
    void saveFileMetadata();
};

#endif
