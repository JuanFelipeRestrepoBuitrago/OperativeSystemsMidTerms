#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

class Utils {
public:
    static size_t getSizeImageRotated(int width, int height);
    static size_t getSizeImageScaled(int width, int height, double scale);
    static size_t roundToNextPowerOfTwo(size_t size);
};

#endif // UTILS_H
