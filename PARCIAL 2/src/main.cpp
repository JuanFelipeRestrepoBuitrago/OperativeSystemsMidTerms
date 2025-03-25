#include "memory/FileManager.h"
#include "core/ImageManager.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <chrono>
#include <cstring>
#include <cmath>

using namespace std;
using namespace std::chrono;

void showUsage() {
    cout << "Usage: ./program <input_file> <output_file> <factor> <-buddy|-no-buddy>" << endl;
    cout << "  <input_file>    Input image file (PNG, BMP, JPG)" << endl;
    cout << "  <output_file>   Output image file" << endl;
    cout << "  <factor>        Rotation angle in degrees" << endl;
    cout << "  -buddy          Use Buddy System for memory allocation" << endl;
    cout << "  -no-buddy       Use new/delete for memory allocation" << endl;
    cout << "  -rotate         Rotate the image" << endl;
    cout << "  -scale          Scale the image" << endl;
}

void showChecklist(const string &inputFile, const string &outputFile, bool useBuddy, double factor) {
    cout << "\n=== Check List ===" << endl;
    cout << "Input file: " << inputFile << endl;
    cout << "Output file: " << outputFile << endl;
    cout << "Rotation factor: " << factor << endl;
    cout << "Allocation mode: " << (useBuddy ? "Buddy System" : "new/delete") << endl;
    cout << "------------------------" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Error: Incorrect number of arguments." << endl;
        showUsage();
        return 1;
    }

    string inputFile = argv[1];
    string outputFile = argv[2];
    double factor = stod(argv[3]);
    string allocationMode = argv[4];
    string operation = argv[5];

    bool useBuddy;
    if (allocationMode == "-buddy") {
        useBuddy = true;
    } else if (allocationMode == "-no-buddy") {
        useBuddy = false;
    } else {
        cerr << "Error: Invalid mode option." << endl;
        showUsage();
        return 1;
    }

    showChecklist(inputFile, outputFile, useBuddy, factor);

    auto start = high_resolution_clock::now();
    TransformationMethod method;
    if (operation == "-rotate") {
        method = TransformationMethod::ROTATION;
    } else if (operation == "-scale") {
        method = TransformationMethod::SCALING;
    } else {
        cerr << "Error: Invalid operation (-rotate or -scale)." << endl;
        showUsage();
        return 1;
    }

    FileManager fm(inputFile, outputFile, method, useBuddy, factor);

    unsigned char** originalPixels = fm.initializeOriginalPixelsFromFile();
    fm.showFileInfo();

    ImageManager imgManager(fm.getWidth(), fm.getHeight(), fm.getChannels());
    unsigned char** transformedPixels = fm.initializeTransformedPixels();

    Pixel fillColor = {0, 0, 0, 255};
    if (operation == "-rotate") {
        imgManager.rotateImage(factor, fillColor, transformedPixels, originalPixels, fm.getTransformedImageWidth(), fm.getTransformedImageHeight());
    } else if (operation == "-scale") {
        imgManager.scaleImage(factor, transformedPixels, originalPixels);
    }
    
    fm.saveImage(transformedPixels, fm.getTransformedImageWidth(), fm.getTransformedImageHeight(), fm.getChannels());

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "\nTotal processing time: " << duration << " ms" << endl;
    if (operation == "-rotate") {
        cout << "\n[INFO] Rotated image successfully saved." << endl;
    } else {
        cout << "\n[INFO] Scaled image successfully saved." << endl;
    }    

    return 0;
}