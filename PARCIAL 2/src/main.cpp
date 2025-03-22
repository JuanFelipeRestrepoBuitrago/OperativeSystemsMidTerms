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
    cout << "Uso: ./main <archivo_entrada> <archivo_salida> <-buddy|-no-buddy>" << endl;
    cout << "  <input_file>   Input image file (PNG, BMP, JPG)" << endl;
    cout << "  <output_file>    Output file for the processed image" << endl;
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


int main(int argc, char* argv[]) {
    // Verificar número de argumentos
    if (argc != 4) {
        cerr << "Error: Incorrect number of arguments." << endl;
        mostrarUso();
        return 1;
    }

    // Parámetros de línea de comandos
    string archivoEntrada = argv[1];
    string archivoSalida = argv[2];
    string modoAsignacion = argv[3];

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

        FileManager fm(archivoEntrada, archivoSalida, TransformationMethod::ROTATION, true);

        // Get the pixels of the image from the file
        unsigned char** pixels = fm.initializeOriginalPixelsFromFile();
        fm.showFileInfo();
        unsigned char** pixelsTransformados = fm.initializeTransformedPixels();

        int offsetX = (fm.getTransformedImageWidth()  - fm.getWidth())  / 2;
        int offsetY = (fm.getTransformedImageHeight() - fm.getHeight()) / 2;

        // Copy the original pixels to the transformed image in the center
        for (int i = 0; i < fm.getHeight(); i++) {
            for (int j = 0; j < fm.getWidth() * fm.getChannels(); j++) {
                pixelsTransformados[i + offsetY][j + offsetX * fm.getChannels()] = pixels[i][j];
            }
        }

        // Guardar imagen procesada
        fm.saveImage(pixelsTransformados, fm.getTransformedImageWidth(), fm.getTransformedImageHeight(), fm.getChannels());
    } else {
        cout << "\n[INFO] Using new/delete for memory assignment." << endl;

        FileManager fm(archivoEntrada, archivoSalida, TransformationMethod::ROTATION, false);

        // // Crear el allocador Buddy System de 32 MB
        // BuddyAllocator allocador(32 * 1024 * 1024);
        unsigned char** pixels = fm.initializeOriginalPixelsFromFile();
        fm.showFileInfo();
        unsigned char** pixelsTransformados = fm.initializeTransformedPixels();

        int offsetX = (fm.getTransformedImageWidth()  - fm.getWidth())  / 2;
        int offsetY = (fm.getTransformedImageHeight() - fm.getHeight()) / 2;

        // Copy the original pixels to the transformed image in the center
        for (int i = 0; i < fm.getHeight(); i++) {
            for (int j = 0; j < fm.getWidth() * fm.getChannels(); j++) {
                pixelsTransformados[i + offsetY][j + offsetX * fm.getChannels()] = pixels[i][j];
            }
        }

        // Guardar imagen procesada
        fm.saveImage(pixelsTransformados, fm.getTransformedImageWidth(), fm.getTransformedImageHeight(), fm.getChannels());
    }

    // Medir tiempo de finalización
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio).count();

    cout << "\nTotal time for processing: " << duracion << " ms" << endl;

    cout << "\n[INFO] Procedure completed successfully." << endl;

    return 0;
}