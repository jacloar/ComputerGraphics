#include "../include/PPM.h"
#include <iostream>
#include <fstream>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){

    std::ifstream inFile;
    inFile.open(fileName.c_str());

    if (inFile.is_open()) {
        std::string line;

        getline(inFile, line);
        getline(inFile, line);
        


        inFile >> m_width;
        inFile >> m_height;
        inFile >> maxPixValue;

        double scale = 255.0 / maxPixValue;

        m_PixelData = new unsigned char[m_width * m_height * 3];
        int number;

        for (int ii = 0; ii < m_width * m_height * 3; ii += 1) {
            inFile >> number;
            m_PixelData[ii] = number * scale;

        }
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

    outFile.open(outputFileName.c_str());

    if (outFile.is_open()) {
        outFile << "P3" << std::endl;
        outFile << m_width << " " << m_height << std::endl;
        outFile << 255 << std::endl;
        for (int ii = 0; ii < getHeight() * getWidth() * 3; ii += 1) {
            outFile << (int) m_PixelData[ii] << std::endl;
        }
        
    }

    outFile.close();
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){

    //decrements every pixel value by 50 with lower bound of zero.
    for (int ii = 0; ii < getHeight() * getWidth() * 3; ii += 1) {

        if (m_PixelData[ii] >= 50) {
            m_PixelData[ii] -= 50;
        }
        else {
            m_PixelData[ii] = 0;
        }
    }
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){

    m_PixelData[((y * getWidth() + x) * 3)] = R;
    m_PixelData[((y * getWidth() + x) * 3) + 1] = G;
    m_PixelData[((y * getWidth() + x) * 3) + 2] = B;
}
