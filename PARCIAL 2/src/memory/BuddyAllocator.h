#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <cstddef>
#include "../utils/Utils.h"
#include <cstdlib>
#include <iostream>

class BuddyAllocator {
public:
    BuddyAllocator(size_t size);
    ~BuddyAllocator();

    void* alloc(size_t size);
private:
    size_t size; // Total size of the managed memory block
    void* baseMemory; // Pointer to the base of the memory block
};

#endif
