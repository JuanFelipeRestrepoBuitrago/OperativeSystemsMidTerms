#include "memory/FileManager.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <chrono>
#include <cstring>
#include <cmath>

using namespace std;
using namespace std::chrono;

// Muestra cómo se usa el programa desde la línea de comandos
void mostrarUso() {
    cout << "Uso: ./main <archivo_entrada> <archivo_salida> <factor> <-buddy|-no-buddy>" << endl;
    cout << "  <input_file>   Input image file (PNG, BMP, JPG)" << endl;
    cout << "  <output_file>    Output file for the processed image" << endl;
    cout << "  <factor>         Factor for the transformation, if rotation, the angle in degrees, if scaling, the scaling factor" << endl;
    cout << "  -buddy             Uses Buddy System for memory assignment" << endl;
    cout << "  -no-buddy          Uses new/delete for memory assignment" << endl;
}

// Muestra una lista de chequeo para verificar que los parámetros son correctos
void mostrarListaChequeo(const string &archivoEntrada, const string &archivoSalida, bool usarBuddy) {
    cout << "\n=== Check List ===" << endl;
    cout << "Input File: " << archivoEntrada << endl;
    cout << "Output File: " << archivoSalida << endl;
    cout << "Assignment Mode: " << (usarBuddy ? "Buddy System" : "new/delete") << endl;
    cout << "------------------------" << endl;
}

void save_rotated(FileManager fm){
    unsigned char** pixels = fm.initializeOriginalPixelsFromFile();
    fm.showFileInfo();
    unsigned char** pixelsTransformados = fm.initializeTransformedPixels();

    
    cout << "\n[INFO] Rotating image..." << endl;
    cout << "Transformation factor: " << fm.getTransformationFactor() << endl;
    double theta = fm.getTransformationFactor() * 2 * M_PI / 360;
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    int origWidth = fm.getWidth();
    int origHeight = fm.getHeight();
    int newWidth = fm.getTransformedImageWidth();
    int newHeight = fm.getTransformedImageHeight();
    int channels = fm.getChannels();

    // Compute centers: original and transformed
    double ocx = origWidth / 2.0;
    double ocy = origHeight / 2.0;
    double ncx = newWidth / 2.0;
    double ncy = newHeight / 2.0;

    // Optionally, initialize the transformed image with a background color (black)
    for (int y = 0; y < newHeight; y++) {
        memset(pixelsTransformados[y], 0, newWidth * channels * sizeof(unsigned char));
    }

    // For each pixel in the new (rotated) image, compute its source coordinates
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            // Coordinates relative to the center of the transformed image
            double x_rel = x - ncx;
            double y_rel = y - ncy;

            // Inverse rotation to get original image coordinates
            double orig_x = cos_theta * x_rel + sin_theta * y_rel + ocx;
            double orig_y = -sin_theta * x_rel + cos_theta * y_rel + ocy;

            // Nearest neighbor interpolation: round to nearest pixel
            int origX = static_cast<int>(round(orig_x));
            int origY = static_cast<int>(round(orig_y));

            // Check bounds: if within the original image, copy the pixel
            if (origX >= 0 && origX < origWidth && origY >= 0 && origY < origHeight) {
                // Note: since pixels are stored as 1D per row, multiply x index by channels
                for (int c = 0; c < channels; c++) {
                    pixelsTransformados[y][x * channels + c] =
                        pixels[origY][origX * channels + c];
                }
            }
            // Else, leave the background color (0) in pixelsTransformados.
        }
    }

    fm.saveImage(pixelsTransformados, fm.getTransformedImageWidth(), fm.getTransformedImageHeight(), fm.getChannels());
}

int main(int argc, char* argv[]) {
    // Verificar número de argumentos
    if (argc != 5) {
        cerr << "Error: Incorrect number of arguments." << endl;
        mostrarUso();
        return 1;
    }

    // Parámetros de línea de comandos
    string archivoEntrada = argv[1];
    string archivoSalida = argv[2];
    double factor = stod(argv[3]);
    string modoAsignacion = argv[4];

    // Verifica si el modo de asignación es válido
    bool usarBuddy = false;
    if (modoAsignacion == "-buddy") {
        usarBuddy = true;
    } else if (modoAsignacion == "-no-buddy") {
        usarBuddy = false;
    } else {
        cerr << "Error: Mode option invalid." << endl;
        mostrarUso();
        return 1;
    }

    // Mostrar lista de chequeo
    mostrarListaChequeo(archivoEntrada, archivoSalida, usarBuddy);

    // Medir el tiempo de ejecución
    auto inicio = high_resolution_clock::now();

    if (usarBuddy) {
        cout << "\n[INFO] Using Buddy System for memory assignment." << endl;

        FileManager fm(archivoEntrada, archivoSalida, TransformationMethod::ROTATION, true, factor);

        save_rotated(fm);
    } else {
        cout << "\n[INFO] Using new/delete for memory assignment." << endl;

        FileManager fm(archivoEntrada, archivoSalida, TransformationMethod::ROTATION, false, factor);

        save_rotated(fm);
    }

    // Medir tiempo de finalización
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio).count();

    cout << "\nTotal time for processing: " << duracion << " ms" << endl;

    cout << "\n[INFO] Procedure completed successfully." << endl;

    return 0;
}