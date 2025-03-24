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
    double theta = -angleDegrees * M_PI / 180.0;
    double cx = width / 2.0, cy = height / 2.0;

    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    double cx_new = newWidth / 2.0;
    double cy_new = newHeight / 2.0;

    initializeBufferWithColor(fillColor, transformedPixels, newWidth, newHeight);

    for (int yNew = 0; yNew < newHeight; ++yNew) {
        for (int xNew = 0; xNew < newWidth; ++xNew) {

            double xRel = xNew - cx_new;
            double yRel = yNew - cy_new;

            double xOrig = xRel * cos_theta - yRel * sin_theta + cx;
            double yOrig = xRel * sin_theta + yRel * cos_theta + cy;

            int x0 = floor(xOrig);
            int y0 = floor(yOrig);
            int x1 = x0 + 1;
            int y1 = y0 + 1;

            if (x0 >= 0 && x1 < width && y0 >= 0 && y1 < height) {
                double dx = xOrig - x0;
                double dy = yOrig - y0;

                for (int c = 0; c < channels; c++) {
                    double pixel = 
                        (1 - dx) * (1 - dy) * originalPixels[y0][x0 * channels + c] +
                        dx * (1 - dy) * originalPixels[y0][x1 * channels + c] +
                        (1 - dx) * dy * originalPixels[y1][x0 * channels + c] +
                        dx * dy * originalPixels[y1][x1 * channels + c];

                    int idxDst = xNew * channels + c;
                    transformedPixels[yNew][idxDst] = static_cast<unsigned char>(round(pixel));
                }
            }
        }
    }
    width = newWidth;
    height = newHeight;
}
