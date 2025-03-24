#include "ImageManager.h"
#include <cmath>

ImageManager::ImageManager(int width, int height, int channels)
    : width(width), height(height), channels(channels) {}


int ImageManager::getWidth() const { return width; }
int ImageManager::getHeight() const { return height; }
int ImageManager::getChannels() const { return channels; }

void ImageManager::initializeBufferWithColor(Pixel fillColor, unsigned char** transformedPixels, int newWidth, int newHeight) {
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
