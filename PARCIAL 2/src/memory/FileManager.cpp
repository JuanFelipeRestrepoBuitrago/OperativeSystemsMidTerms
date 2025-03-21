#include "FileManager.h"

FileManager::FileManager(const std::string& readFilePath, const std::string& writeFilePath, TransformationMethod transformationMethod, bool buddyAllocatorUsage) {
    /**
     * Constructor for the FileManager class.
     * 
     * @param readFilePath: The path to the file to be read.
     * @param writeFilePath: The path to the file to be written.
     * @param transformationMethod: The method to be used for transforming the image.
     * @param buddyAllocatorUsage: Whether to use the buddy allocator for memory management.
     */
    this -> readFilePath = readFilePath;
    this -> writeFilePath = writeFilePath;
    this -> transformationMethod = transformationMethod;
    this -> buddyAllocatorUsage = buddyAllocatorUsage;
    this -> allocatorOriginalImage = nullptr;
    this -> allocatorTransformedImage = nullptr;
    this -> originalPixels = nullptr;
    this -> transformedPixels = nullptr;
    this -> scaleFactor = nullptr;
    this -> width = 0;
    this -> height = 0;
}

FileManager::FileManager(const std::string& readFilePath, const std::string& writeFilePath, TransformationMethod transformationMethod, bool buddyAllocatorUsage, double* scaleFactor) {
    /**
     * Constructor for the FileManager class.
     * 
     * @param readFilePath: The path to the file to be read.
     * @param writeFilePath: The path to the file to be written.
     * @param transformationMethod: The method to be used for transforming the image.
     * @param buddyAllocatorUsage: Whether to use the buddy allocator for memory management.
     * @param scaleFactor: The factor by which the image will be scaled, null if not scaling.
     */
    this -> readFilePath = readFilePath;
    this -> writeFilePath = writeFilePath;
    this -> transformationMethod = transformationMethod;
    this -> buddyAllocatorUsage = buddyAllocatorUsage;
    this -> allocatorOriginalImage = nullptr;
    this -> allocatorTransformedImage = nullptr;
    this -> originalPixels = nullptr;
    this -> transformedPixels = nullptr;
    this -> scaleFactor = scaleFactor;
    this -> width = 0;
    this -> height = 0;
}

FileManager::~FileManager() {
    /**
     * Destructor for the FileManager class.
     */
    // Only deallocate if pointers are valid
    if (allocatorOriginalImage) {
        delete allocatorOriginalImage;
    } else if (originalPixels != nullptr) { // Add null check
        deallocateMemory(originalPixels);
    }
    
    if (allocatorTransformedImage) {
        delete allocatorTransformedImage;
    } else if (transformedPixels != nullptr) { // Add null check
        deallocateMemory(transformedPixels);
    }
}

unsigned char** FileManager::allocateMemory(int width, int height, BuddyAllocator* allocator) {
    if (buddyAllocatorUsage && allocator != nullptr) {
        // Calculate total memory needed for row pointers + pixel data
        size_t row_pointers_size = height * sizeof(unsigned char*);
        size_t pixels_size = height * width * sizeof(unsigned char);
        size_t total_size = row_pointers_size + pixels_size;

        // Allocate a single block from the buddy allocator
        void* block = allocator->alloc(total_size);
        if (!block) {
            std::cerr << "Error: BuddyAllocator failed to allocate memory.\n";
            exit(1);
        }

        // Set up row pointers and pixel data in the block
        unsigned char** rows = static_cast<unsigned char**>(block);
        unsigned char* pixels = reinterpret_cast<unsigned char*>(rows + height);

        for (int i = 0; i < height; ++i) {
            rows[i] = pixels + i * width;
        }

        return rows;
    } else {
        // Fallback to standard allocation
        unsigned char** pixels = new unsigned char*[height];
        for (int i = 0; i < height; ++i) {
            pixels[i] = new unsigned char[width];
        }
        return pixels;
    }
}

void FileManager::deallocateMemory(unsigned char** pixels) {
    /**
     * Deallocate the memory used by the pixels of the image.
     * 
     * @param pixels: The pointer to the memory to be deallocated.
     */
    size_t length = sizeof(pixels) / sizeof(pixels[0]);
    for (int i = 0; i < length; ++i) {
        delete[] pixels[i];
    }
    delete[] pixels;
}

void FileManager::getFileMetadata() {
    /**
     * Get the metadata of the image file.
     */
    int channels;
    if (!stbi_info(readFilePath.c_str(), &width, &height, &channels)) {
        std::cerr << "Error: Could not read image metadata.\n";
        exit(1);
    }
}

unsigned char** FileManager::initializeOriginalPixelsFromFile() {
    /**
     * Read the pixels of the image from the file.
     * 
     * @return A pointer to the pixels of the image.
     */
    if(width == 0 || height == 0) {
        getFileMetadata();
    }
    if (buddyAllocatorUsage) {
        allocatorOriginalImage = new BuddyAllocator(height * sizeof(unsigned char*) + height * width * sizeof(unsigned char));
    }
    unsigned char* buffer = stbi_load(readFilePath.c_str(), &width, &height, nullptr, 0);
    if (!buffer) {
        std::cerr << "Error: Could not read image pixels.\n";
        exit(1);
    }

    originalPixels = allocateMemory(width, height, allocatorOriginalImage);
    for (int i = 0; i < height; ++i) {
        memcpy(originalPixels[i], buffer + i * width, width);
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
    if(width == 0 || height == 0) {
        getFileMetadata();
    }
    switch (transformationMethod) {
        case TransformationMethod::ROTATION:
            Utils::getSizeImageRotated(width, height);
            break;
        case TransformationMethod::SCALING:
            Utils::getSizeImageScaled(width, height, *scaleFactor);
            break;
    }

    if (buddyAllocatorUsage) {
        allocatorTransformedImage = new BuddyAllocator(height * sizeof(unsigned char*) + height * width * sizeof(unsigned char));
    }

    transformedPixels = allocateMemory(width, height, allocatorTransformedImage);

    return transformedPixels;
}

void FileManager::saveImage(unsigned char** data) {
    /**
     * Save the transformed image to a file.
     * 
     * @param data: The pointer to the pixels of the transformed image.
     */
    unsigned char* buffer = new unsigned char[width * height];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            buffer[i * width + j] = data[i][j];
        }
    }

    if (!stbi_write_png(writeFilePath.c_str(), width, height, 1, buffer, width)) {
        std::cerr << "Error: Could not write image to file.\n";
        delete[] buffer;
        exit(1);
    }

    delete[] buffer;
}
