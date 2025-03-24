#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <vector>
#include <string>

struct Pixel {
    unsigned char r, g, b, a;
};

class ImageManager {
public:
    ImageManager(int width, int height, int channels);

    void rotateImage(float angleDegrees, Pixel fillColor, unsigned char** transformedPixels, unsigned char** originalPixels, int newWidth, int newHeight);

    int getWidth() const;
    int getHeight() const;
    int getChannels() const;

private:
    int width;
    int height;
    int channels;

    void initializeBufferWithColor(Pixel fillColor, unsigned char** transformedPixels, int newWidth, int newHeight);
    
};

#endif
