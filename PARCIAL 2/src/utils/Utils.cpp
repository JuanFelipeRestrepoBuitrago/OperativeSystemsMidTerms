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