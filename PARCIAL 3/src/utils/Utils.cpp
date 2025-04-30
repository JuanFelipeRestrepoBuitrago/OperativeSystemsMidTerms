#include "Utils.h"
#include <cmath>

void Utils::getSizeImageRotated(int& width, int& height) {
    double cx = width / 2.0, cy = height / 2.0;

    double R = sqrt(cx * cx + cy * cy);
    int newWidth = ceil(2 * R);
    int newHeight = ceil(2 * R);
    
    width = newWidth;
    height = newHeight;
}

void Utils::getSizeImageScaled(int &width, int &height, double scale) {
    /**
     * Calculate the new size of each side of the image after scaling it by the given factor.
     * 
     * @param width: The width of the image.
     * @param height: The height of the image.
     * @param scale: The factor by which the image will be scaled.
     */
    
    // Calculate the new width and height
    width = std::ceil(width * scale);
    height = std::ceil(height * scale);
}


size_t Utils::roundToNextPowerOfTwo(size_t size) {
    /**
     * Round the given size to the next power of two.
     * 
     * @param size: The size to be rounded.
     * @return The next power of two greater than or equal to the given size.
     */
    if (size == 0) return 1;
    size_t power = 1;
    while (power < size) power <<= 1; // Equivalent to power *= 2
    return power;
}