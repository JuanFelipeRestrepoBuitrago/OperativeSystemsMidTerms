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
    this -> transformedImageWidth = 0;
    this -> transformedImageHeight = 0;
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
        std::cerr << "Error: Could not read image metadata.\n";
        exit(1);
    }
}

void FileManager::showFileInfo() const {
    std::cout << "Dimensions: " << width << " x " << height << std::endl;
    std::cout << "Channels: " << channels << std::endl;
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
        size_t size = height * sizeof(unsigned char*) + height * width * channels * sizeof(unsigned char);
        allocatorOriginalImage = new BuddyAllocator(size);
    }
    unsigned char* buffer = stbi_load(readFilePath.c_str(), &width, &height, &channels, 0);
    if (!buffer) {
        std::cerr << "Error: Could not read image pixels.\n";
        exit(1);
    }

    originalPixels = allocateMemory(width * channels, height, allocatorOriginalImage);
    for (int i = 0; i < height; ++i) {
        memcpy(originalPixels[i], buffer + i * width * channels, width * channels);
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
            if (scaleFactor == nullptr) {
                std::cerr << "Error: Scale factor is not provided.\n";
                exit(1);
            }
            Utils::getSizeImageScaled(transformedImageWidth, transformedImageHeight, *scaleFactor);
            break;
    }

    if (buddyAllocatorUsage) {
        size_t size = transformedImageHeight * sizeof(unsigned char*) + transformedImageHeight * transformedImageWidth * channels * sizeof(unsigned char);
        allocatorTransformedImage = new BuddyAllocator(size);
    }

    transformedPixels = allocateMemory(transformedImageWidth, transformedImageHeight, allocatorTransformedImage);

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

    if (!stbi_write_png(writeFilePath.c_str(), width, height, channels, buffer, width * channels)) {
        std::cerr << "Error: Could not write image to file.\n";
        delete[] buffer;
        exit(1);
    }

    delete[] buffer;
}
