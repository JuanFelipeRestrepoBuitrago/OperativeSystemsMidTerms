#include "ImageManager.h"
#include <cmath>

ImageManager::ImageManager() : width(0), height(0), channels(0) {}

int ImageManager::getWidth() const { return width; }
int ImageManager::getHeight() const { return height; }
int ImageManager::getChannels() const { return channels; }

void ImageManager::loadDataFromPixels(unsigned char** pixels, int w, int h, int ch) {
    width = w;
    height = h;
    channels = ch;

    data.resize(height, std::vector<Pixel>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = x * channels;
            unsigned char alpha = 255;
            if(channels == 4){
                alpha = pixels[y][idx + 3];
            }
            data[y][x] = {
                pixels[y][idx],
                pixels[y][idx + 1],
                pixels[y][idx + 2],
                alpha
            };
        }
    }
}

void ImageManager::rotateImage(float angleDegrees, Pixel fillColor) {
    double theta = angleDegrees * M_PI / 180.0;
    double cx = width / 2.0, cy = height / 2.0;

    double R = sqrt(cx * cx + cy * cy);
    int newWidth = ceil(2 * R);
    int newHeight = ceil(2 * R);

    std::vector<std::vector<Pixel>> rotatedData(newHeight, std::vector<Pixel>(newWidth, fillColor));

    double cx_new = newWidth / 2.0;
    double cy_new = newHeight / 2.0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double xRel = x - cx;
            double yRel = y - cy;

            double xRot = xRel * cos(theta) - yRel * sin(theta);
            double yRot = xRel * sin(theta) + yRel * cos(theta);

            int xNew = round(cx_new + xRot);
            int yNew = round(cy_new + yRot);

            if (xNew >= 0 && xNew < newWidth && yNew >= 0 && yNew < newHeight) {
                rotatedData[yNew][xNew] = data[y][x];
            }
        }
    }
    data = rotatedData;
    width = newWidth;
    height = newHeight;
}

unsigned char** ImageManager::exportDataToPixels() {
    unsigned char** pixels = new unsigned char*[height];
    for (int y = 0; y < height; ++y) {
        pixels[y] = new unsigned char[width * channels];
        for (int x = 0; x < width; ++x) {
            int idx = x * channels;
            pixels[y][idx] = data[y][x].r;
            pixels[y][idx + 1] = data[y][x].g;
            pixels[y][idx + 2] = data[y][x].b;
            if (channels == 4)
                pixels[y][idx + 3] = data[y][x].a;
        }
    }
    return pixels;
}
