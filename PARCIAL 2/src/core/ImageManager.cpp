#include "ImageManager.h"
#include <cmath>

/**
 * Constructor for ImageManager.
 *
 * @param width The width of the image.
 * @param height The height of the image.
 * @param channels Number of color channels.
 */

ImageManager::ImageManager(int width, int height, int channels)
    : width(width), height(height), channels(channels) {}

/**
 * Get the current width of the image.
 *
 * @return The width of the image.
 */
int ImageManager::getWidth() const { return width; }

/**
 * Get the current height of the image.
 *
 * @return The height of the image.
 */
int ImageManager::getHeight() const { return height; }

/**
 * Get the number of color channels of the image.
 *
 * @return The number of color channels.
 */
int ImageManager::getChannels() const { return channels; }


void ImageManager::initializeBufferWithColor(Pixel fillColor, unsigned char** transformedPixels, int newWidth, int newHeight) {
    /**
     * Initialize the buffer with a given color.
     *
     * @param fillColor The color to fill the buffer with.
     * @param transformedPixels The buffer to be filled.
     * @param newWidth The width of the buffer.
     * @param newHeight The height of the buffer.
     */
    for(int y = 0; y < newHeight; y++){
        for(int x = 0; x < newWidth; x++){
            int idx = x * channels;
            transformedPixels[y][idx] = fillColor.r;
            transformedPixels[y][idx + 1] = fillColor.g;
            transformedPixels[y][idx + 2] = fillColor.b;
            if(channels == 4)
                transformedPixels[y][idx + 3] = fillColor.a;
        }
    }
}

void ImageManager::rotateImage(float angleDegrees, Pixel fillColor, unsigned char** transformedPixels, unsigned char** originalPixels,int newWidth, int newHeight) {
    /**
     * Rotate the image by the specified angle and store the result in the provided buffer.
     *
     * @param angleDegrees The rotation angle in degrees.
     * @param fillColor The background color used for pixels without a corresponding original pixel.
     * @param transformedPixels The buffer to store the rotated image.
     * @param originalPixels The original image buffer.
     * @param newWidth The width of the rotated image.
     * @param newHeight The height of the rotated image.
     */
    double theta = angleDegrees * M_PI / 180.0;
    double cx = width / 2.0, cy = height / 2.0;

    double cx_new = newWidth / 2.0;
    double cy_new = newHeight / 2.0;

    initializeBufferWithColor(fillColor, transformedPixels, newWidth, newHeight);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double xRel = x - cx;
            double yRel = y - cy;

            double xRot = xRel * cos(theta) - yRel * sin(theta);
            double yRot = xRel * sin(theta) + yRel * cos(theta);

            int xNew = round(cx_new + xRot);
            int yNew = round(cy_new + yRot);

            if (xNew >= 0 && xNew < newWidth && yNew >= 0 && yNew < newHeight) {
                int idxSrc = x * channels;
                int idxDst = xNew * channels;
                for (int c = 0; c < channels; c++) {
                    transformedPixels[yNew][idxDst + c] = originalPixels[y][idxSrc + c];
                }
            }
        }
    }
    width = newWidth;
    height = newHeight;
}
