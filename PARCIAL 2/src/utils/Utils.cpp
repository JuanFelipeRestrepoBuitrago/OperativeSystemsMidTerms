#include "Utils.h"
#include <cmath>

void Utils::getSizeImageRotated(int& width, int& height, double angle_degrees) {
    double angle = angle_degrees * M_PI / 180.0;
    double cos_theta = fabs(cos(angle));
    double sin_theta = fabs(sin(angle));
    
    // Calculate new dimensions based on rotated bounds
    int new_width = static_cast<int>(ceil(width * cos_theta + height * sin_theta));
    int new_height = static_cast<int>(ceil(width * sin_theta + height * cos_theta));
    
    width = new_width;
    height = new_height;
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