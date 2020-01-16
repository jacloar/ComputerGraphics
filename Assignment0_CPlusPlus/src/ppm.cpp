#include "PPM.h"
#include <fstream>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){
    std::ifstream inFile;
    inFile.open(fileName);

    if (inFile.is_open()) {
        std::string line;

        if (getline(inFile, line)) {
            m_width = std::stoi(line, nullptr, 10);
        }

        if (getline(inFile, line)) {
            m_height = std::stoi(line, nullptr, 10);
        }

        m_PixelData = new unsigned char[m_width * m_height * 3];
    }

    inFile.close();
}

// Destructor clears any memory that has been allocated
PPM::~PPM() {
    delete[] m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){
    std::ofstream outFile;

    outFile.open(outputFileName);
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){
    // TODO: Output a 'filtered' PPM image.
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
    m_PixelData[((y * getWidth() + x) * 3)] = R;
    m_PixelData[((y * getWidth() + x) * 3) + 1] = G;
    m_PixelData[((y * getWidth() + x) * 3) + 2] = B;
}
