#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <vector>
#include <string>

struct Pixel {
    unsigned char r, g, b, a;
};

class ImageManager {
public:
    ImageManager();

    void loadDataFromPixels(unsigned char** pixels, int width, int height, int channels);
    void rotateImage(float angleDegrees, Pixel fillColor);
    unsigned char** exportDataToPixels();

    int getWidth() const;
    int getHeight() const;
    int getChannels() const;

private:
    int width;
    int height;
    int channels;
    std::vector<std::vector<Pixel>> data;
    
};

#endif
