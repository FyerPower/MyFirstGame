#pragma once

#include "logger.hpp";

// This is to get malloc
#include <stdlib.h>;

// This is to get memset
#include <string.h>;

// ###############################################
//                     Functions
// ###############################################

struct BumpAllocator {
    size_t capacity;
    size_t used;
    char* memory;
};

BumpAllocator make_bump_allocator(size_t size)
{
    BumpAllocator ba = {};
    ba.memory = (char*)malloc(size);

    // If our memory was properly allocated
    if (ba.memory) {
        // Tell the Bump Allocator the size of memory
        ba.capacity = size;

        // Ensure that our memory is 0
        memset(ba.memory, 0, size);
    }
    // If our memory was not properly allocated, lets error out
    else {
        FP_ASSERT(false, "Failed to allocate memory.");
    }
}

char* bump_alloc(BumpAllocator* bumpAllocator, size_t size)
{
    char* result = nullptr;

    // Ensure that the first 4 bits are set to 0
    // TODO: Should this actually be 15 instead of 7?
    // https://www.youtube.com/watch?v=S96DCaV5X2M&lc=UgzB9tnvc0BpUbuGe9N4AaABAg.9vEtOz7Cab59vhQkRoUQ5h
    size_t allignedSize = (size + 7) & ~7;

    // if memory has enough room for what we want to alloc, we're good!
    if (bumpAllocator->used + allignedSize <= bumpAllocator->capacity) {
        result = bumpAllocator->memory + bumpAllocator->used;
        bumpAllocator->used += allignedSize;
    } else {
        FP_ASSERT(false, "BumpAllocator is Full");
    }

    return result;
}