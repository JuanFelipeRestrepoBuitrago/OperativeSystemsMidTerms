#include "Utils.h"
#include <cmath>

size_t Utils::getSizeImageRotated(int width, int height) {
    /**
     * Calculate the new size of the image after rotating it by 45 degrees.
     * 
     * @param width: The width of the original image.
     * @param height: The height of the original image.
     * @return The new size of the image after rotating it.
     */
    // Calculate the hipotenuse of the image
    double hypotenuse = std::sqrt(std::pow(width / 2, 2) + std::pow(height / 2, 2));

    // Calculate the new width and height
    int newWidth = std::ceil(2 * hypotenuse);
    int newHeight = std::ceil(2 * hypotenuse);

    return newWidth * newHeight;
}

size_t Utils::getSizeImageScaled(int width, int height, double scale) {
    /**
     * Calculate the new size of the image after scaling it by the given factor.
     * 
     * @param width: The width of the original image.
     * @param height: The height of the original image.
     * @param scale: The factor by which the image will be scaled.
     * @return The new size of the image after scaling it.
     */
    
    // Calculate the new width and height
    int newWidth = std::ceil(width * scale);
    int newHeight = std::ceil(height * scale);

    return newWidth * newHeight;
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