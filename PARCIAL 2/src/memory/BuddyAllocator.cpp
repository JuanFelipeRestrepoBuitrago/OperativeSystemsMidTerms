#include "BuddyAllocator.h"

using namespace std;

BuddyAllocator::BuddyAllocator(size_t size) {
    /**
     * Constructor for the BuddyAllocator class. Assigns a memory block of the specified size.
     * 
     * @param size: The size of the memory block to be managed.
     */
    this->size = Utils::roundToNextPowerOfTwo(size); // Force initial size to be a power of two
    baseMemory = std::malloc(this->size);
    if (!baseMemory) {
        cerr << "Error: Memory allocation failed.\n";
        exit(1);
    }
}

BuddyAllocator::~BuddyAllocator() {
    /**
     * Destructor for the BuddyAllocator class. Frees the memory block.
     */
    std::free(baseMemory);
}

void* BuddyAllocator::alloc(size_t size) {
    /**
     * Allocate a block of memory of the specified size.
     * 
     * @param size: The size of the memory block to be allocated.
     * @return A pointer to the allocated memory block.
     */
    size_t roundedSize = Utils::roundToNextPowerOfTwo(size);
    if (roundedSize > this->size) {
        cerr << "Error: Requested size (" << roundedSize 
             << " bytes) exceeds allocator capacity (" 
             << this->size << " bytes).\n";
        return nullptr;
    }
    return baseMemory; // Simplified for single-block allocation
}