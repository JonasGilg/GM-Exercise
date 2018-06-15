#pragma once

#include <cstddef>

class PickingOffsetManager {
    static unsigned int offsetCounter;

public:
    static int getNewOffset(size_t size) {
        unsigned int currOffset = offsetCounter;
        offsetCounter += size;
        return currOffset;
    }
};