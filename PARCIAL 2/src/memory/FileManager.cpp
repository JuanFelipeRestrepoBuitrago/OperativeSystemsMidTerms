#include "FileManager.h"
#include <omp.h>
FileManager::FileManager(const std::string& readFilePath, const std::string& writeFilePath, TransformationMethod transformationMethod, bool buddyAllocatorUsage, double factor) {
    /**
     * Constructor for the FileManager class.
     * 
     * @param readFilePath: The path to the file to be read.
     * @param writeFilePath: The path to the file to be written.
     * @param transformationMethod: The method to be used for transforming the image.
     * @param buddyAllocatorUsage: Whether to use the buddy allocator for memory management.
     * @param factor: The factor by which the image will be transformed. If degrees, give the angle in degrees. If scaling, give the scaling factor.
     */
    this -> readFilePath = readFilePath;
    this -> writeFilePath = writeFilePath;
    this -> transformationMethod = transformationMethod;
    this -> buddyAllocatorUsage = buddyAllocatorUsage;
    this -> allocatorOriginalImage = nullptr;
    this -> allocatorTransformedImage = nullptr;
    this -> originalPixels = nullptr;
    this -> transformedPixels = nullptr;
    this -> transformationFactor = factor;
    this -> width = 0;
    this -> height = 0;
    this -> transformedImageWidth = 0;
    this -> transformedImageHeight = 0;
}

FileManager::~FileManager() {
    /**
     * Destructor for the FileManager class.
     */
    // Deallocate original image memory
    if (allocatorOriginalImage) {
        delete allocatorOriginalImage;
        allocatorOriginalImage = nullptr;
        originalPixels = nullptr; // Avoid dangling pointer
    } else if (originalPixels != nullptr) {
        deallocateMemory(originalPixels, height);
        originalPixels = nullptr;
    }

    // Deallocate transformed image memory
    if (allocatorTransformedImage) {
        delete allocatorTransformedImage;
        allocatorTransformedImage = nullptr;
        transformedPixels = nullptr; // Avoid dangling pointer
    } else if (transformedPixels != nullptr) {
        deallocateMemory(transformedPixels, transformedImageHeight);
        transformedPixels = nullptr;
    }
}

unsigned char** FileManager::allocateMemory(int width, int height, BuddyAllocator* allocator) {
    unsigned char** pixels = new unsigned char*[height];
    for (int i = 0; i < height; ++i) {
        pixels[i] = new unsigned char[width];
    }
    return pixels;
}

void FileManager::deallocateMemory(unsigned char** pixels, int height) {
    /**
     * Deallocate the memory used by the pixels of the image.
     * 
     * @param pixels: The pointer to the memory to be deallocated.
     * @param height: The height of the image.
     */
    if (pixels == nullptr || height <= 0) return;
    for (int i = 0; i < height; i++) {
        delete[] pixels[i];
    }
    delete[] pixels;
}

void FileManager::getFileMetadata() {
    /**
     * Get the metadata of the image file.
     */
    if (!stbi_info(readFilePath.c_str(), &width, &height, &channels)) {
        std::cerr << "Error: Could not read image metadata of " << readFilePath << ".\n";
        exit(1);
    }
}

void FileManager::showFileInfo() const {
    std::cout << "Dimensions: " << width << " x " << height << std::endl;
    std::cout << "Channels: " << channels << std::endl;
}

unsigned char** FileManager::initializeOriginalPixelsFromFile(bool parallelize) {
    /**
     * Read the pixels of the image from the file.
     * 
     * @return A pointer to the pixels of the image.
     */
    if(width == 0 || height == 0) {
        getFileMetadata();
    }
    if (buddyAllocatorUsage) {
        size_t size = height * sizeof(unsigned char*) + height * width * channels * sizeof(unsigned char);
        allocatorOriginalImage = new BuddyAllocator(size);
    }
    unsigned char* buffer = stbi_load(readFilePath.c_str(), &width, &height, &channels, 0);
    if (!buffer) {
        std::cerr << "Error: Could not read image pixels of " << readFilePath << ".\n";
        exit(1);
    }

    originalPixels = allocateMemory(width * channels, height, allocatorOriginalImage);
    if (parallelize){
        #pragma omp parallel for collapse(2) shared(originalPixels, buffer) num_threads(4)
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width * channels; ++j) {
                originalPixels[i][j] = buffer[i * width * channels + j];
            }
        }
    }
    else{
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width * channels; ++j) {
                originalPixels[i][j] = buffer[i * width * channels + j];
            }
        }
    }

    stbi_image_free(buffer);

    return originalPixels;
}

unsigned char** FileManager::initializeTransformedPixels() {
    /**
     * Get the pixels of the transformed image.
     * 
     * @return A pointer to the pixels of the transformed image.
     */
    transformedImageWidth = width;
    transformedImageHeight = height;

    switch (transformationMethod) {
        case TransformationMethod::ROTATION:
            Utils::getSizeImageRotated(transformedImageWidth, transformedImageHeight);
            break;
        case TransformationMethod::SCALING:
            Utils::getSizeImageScaled(transformedImageWidth, transformedImageHeight, transformationFactor);
            break;
    }

    if (buddyAllocatorUsage) {
        size_t size = transformedImageHeight * sizeof(unsigned char*) + transformedImageHeight * transformedImageWidth * channels * sizeof(unsigned char);
        allocatorTransformedImage = new BuddyAllocator(size);
    }

    transformedPixels = allocateMemory(transformedImageWidth * channels, transformedImageHeight, allocatorTransformedImage);

    return transformedPixels;
}

void FileManager::saveImage(unsigned char** data, int width, int height, int channels) {
    /**
     * Save the transformed image to a file.
     * 
     * @param data: The pointer to the pixels of the transformed image.
     */
    unsigned char* buffer = new unsigned char[width * height * channels];
    for (int i = 0; i < height; i++) {
        memcpy(buffer + i * width * channels, data[i], width * channels);
    }

    if (!stbi_write_jpg(writeFilePath.c_str(), width, height, channels, buffer, width * channels)) {
        std::cerr << "Error: Could not write image to file.\n";
        delete[] buffer;
        exit(1);
    }

    delete[] buffer;
}
