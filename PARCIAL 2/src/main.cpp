#include "memory/FileManager.h"
#include "core/ImageManager.h"

#define _USE_MATH_DEFINES
#include <iostream>
#include <chrono>
#include <cstring>
#include <cmath>
#include <sys/resource.h>

using namespace std;
using namespace std::chrono;

#define PROGRAM_VERSION "v1.0.0"

// ANSI Colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define BOLDWHITE "\033[1m\033[37m"
#define BOLDCYAN "\033[1m\033[36m"

// Show usage instructions
void showUsage()
{
    cout << BOLDCYAN << "\n📘 IMAGE PROCESSING PROGRAM " << PROGRAM_VERSION << RESET << endl;
    cout << BOLDWHITE << "\nUsage:" << RESET << endl;
    cout << "  ./program <input_file> <output_file> <factor> <-buddy|-no-buddy> <-rotate|-scale> <-p | -s>" << endl;

    cout << YELLOW << "\nParameters:" << RESET << endl;
    cout << "  <input_file>     📥 Input image file (PNG, JPG, BMP)" << endl;
    cout << "  <output_file>    💾 Output image file" << endl;
    cout << "  <factor>         🔁 Rotation angle or scaling factor" << endl;
    cout << "  -buddy           🔧 Use Buddy System memory management" << endl;
    cout << "  -no-buddy        ⚙️  Use new/delete memory management" << endl;
    cout << "  -rotate          🔄 Rotate the image" << endl;
    cout << "  -scale           📐 Scale the image" << endl;
    cout << "  -p               📐 Parallelize Process" << endl;
    cout << "  -s               📐 Sequential Process" << endl;

    cout << BOLDWHITE << "\nExample:" << RESET << endl;
    cout << "  ./out/program ./src/tests/images/test.jpg ./out/scaled.jpg 1.5 -buddy -scale" << endl;
    cout << "  ./out/program ./src/tests/images/test.jpg ./out/rotated.jpg 45 -no-buddy -rotate" << endl;
}

// Show version only
void showVersion()
{
    cout << GREEN << "🧩 Image Processing Program - Version " << PROGRAM_VERSION << RESET << endl;
}

// Show a summary of the configuration
void showChecklist(const string &inputFile, const string &outputFile, bool useBuddy, double factor, const string &operation)
{
    cout << BOLDWHITE << "\n🧾 CONFIGURATION SUMMARY" << RESET << endl;
    cout << CYAN << "📥 Input file:        " << RESET << inputFile << endl;
    cout << CYAN << "💾 Output file:       " << RESET << outputFile << endl;
    cout << CYAN << "🔁 Operation:         " << RESET << (operation == "-rotate" ? "Rotation" : "Scaling") << endl;
    cout << CYAN << "📏 Factor:            " << RESET << factor << (operation == "-rotate" ? " degrees" : "x") << endl;
    cout << CYAN << "🔧 Memory mode:       " << RESET << (useBuddy ? "Buddy System" : "new/delete") << endl;
    cout << "-------------------------------" << endl;
}

// Get memory usage in KB
long getMemoryUsageKB()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
#if defined(__APPLE__) && defined(__MACH__)
    return usage.ru_maxrss / 1024;
#else
    return usage.ru_maxrss;
#endif
}

int main(int argc, char *argv[])
{
    // Handle -h / --help
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        showUsage();
        return 0;
    }

    // Handle -v / --version
    if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0))
    {
        showVersion();
        return 0;
    }

    // Require 6 arguments
    if (argc != 7)
    {
        cerr << RED << "\n❌ Error: Incorrect number of arguments." << RESET << endl;
        showUsage();
        return 1;
    }

    string inputFile = argv[1];
    string outputFile = argv[2];
    double factor = stod(argv[3]);
    string allocationMode = argv[4];
    string operation = argv[5];
    string parallelize_flag = argv[6];

    bool useBuddy;
    if (allocationMode == "-buddy")
    {
        useBuddy = true;
    }
    else if (allocationMode == "-no-buddy")
    {
        useBuddy = false;
    }
    else
    {
        cerr << RED << "\n❌ Error: Invalid memory mode." << RESET << endl;
        showUsage();
        return 1;
    }

    if (operation != "-rotate" && operation != "-scale")
    {
        cerr << RED << "\n❌ Error: Invalid operation (-rotate or -scale)." << RESET << endl;
        showUsage();
        return 1;
    }

    showChecklist(inputFile, outputFile, useBuddy, factor, operation);

    auto start = high_resolution_clock::now();
    long memoryBefore = getMemoryUsageKB();

    TransformationMethod method = (operation == "-rotate") ? TransformationMethod::ROTATION : TransformationMethod::SCALING;

    FileManager fm(inputFile, outputFile, method, useBuddy, factor);
    unsigned char **originalPixels = fm.initializeOriginalPixelsFromFile();
    if (!originalPixels)
    {
        cerr << RED << "\n❌ Error: Failed to read image: " << inputFile << RESET << endl;
        cerr << RED << "🛠 STB Error: " << stbi_failure_reason() << RESET << endl;
        return 1;
    }

    fm.showFileInfo();

    ImageManager imgManager(fm.getWidth(), fm.getHeight(), fm.getChannels());
    unsigned char **transformedPixels = fm.initializeTransformedPixels();

    Pixel fillColor = {0, 0, 0, 255};

    if (operation == "-rotate")
    {
        if (parallelize_flag == "-p")
        {
            imgManager.rotateImageParallelize(factor, fillColor, transformedPixels, originalPixels,
                                               fm.getTransformedImageWidth(), fm.getTransformedImageHeight());
        }
        else
        {
            imgManager.rotateImage(factor, fillColor, transformedPixels, originalPixels,
                                   fm.getTransformedImageWidth(), fm.getTransformedImageHeight());
        }
    }
    else
    {
        if (parallelize_flag == "-p")
        {
            imgManager.scaleImageParallelize(factor, transformedPixels, originalPixels);
        }
        else
        {
            imgManager.scaleImage(factor, transformedPixels, originalPixels);
        }
    }

    fm.saveImage(transformedPixels,
                 fm.getTransformedImageWidth(), fm.getTransformedImageHeight(), fm.getChannels());

    long memoryAfter = getMemoryUsageKB();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    // Final output
    cout << GREEN << "\n📊 IMAGE PROCESSING REPORT" << RESET << endl;
    cout << CYAN << "📐 Original dimensions:   " << RESET << fm.getWidth() << " x " << fm.getHeight() << endl;
    cout << CYAN << "📏 Final dimensions:      " << RESET << fm.getTransformedImageWidth() << " x " << fm.getTransformedImageHeight() << endl;
    cout << CYAN << "🎨 Channels:              " << RESET << fm.getChannels() << endl;
    if (operation == "-rotate")
    {
        
        cout << CYAN << "🔄 Rotation angle:        " << RESET << factor << "°" << endl;
    }
    else
    {
        cout << CYAN << "📐 Scaling factor:        " << RESET << factor << "x" << endl;
    }

    cout << CYAN << "📐 Parallelism: " << RESET << (parallelize_flag == "-p" ? "YES" : "NO") << endl;


    cout << BOLDWHITE << "\n⚙️ PERFORMANCE STATS" << RESET << endl;
    cout << YELLOW << "⏱️ Processing time:       " << RESET << duration << " ms" << endl;
    cout << YELLOW << "📊 Memory used:           " << RESET << (memoryAfter - memoryBefore) << " KB" << endl;
    cout << YELLOW << "🔧 Memory mode:           " << RESET << (useBuddy ? "Buddy System" : "new/delete") << endl;

    cout << GREEN << "\n✅ Image successfully saved to: " << RESET << outputFile << endl;

    return 0;
}
