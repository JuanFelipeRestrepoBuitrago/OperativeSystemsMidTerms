#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

class Utils {
public:
    static void getSizeImageRotated(int &width, int &height);
    static void getSizeImageScaled(int &width, int &height, double scale);
    static size_t roundToNextPowerOfTwo(size_t size);
};

#endif
