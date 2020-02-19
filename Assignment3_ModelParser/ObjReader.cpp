#include "ObjReader.h"
#include "qstring.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

ObjReader::ObjReader() {

}

ObjReader::ObjReader(std::string fileName) {

	readFile(fileName);
	
	std::cout << "verts: " << std::endl;
	for (int i = 0; i < vertices.size(); i += 1) {
		std::cout << vertices.at(i) << ", ";
	}

	std::cout << "faces: " << std::endl;
	for (int i = 0; i < faces.size(); i += 1) {
		std::cout << faces.at(i) << ", ";
	}
}

QVector<float> ObjReader::getVertices() {
	return vertices;
}

QVector<unsigned int> ObjReader::getFaces() {
	return faces;
}


void ObjReader::readFile(std::string fileName) {

	std::ifstream inFile;
	inFile.open(fileName.c_str());

	if (inFile.is_open()) {
		while (!inFile.eof()) {

			std::string line;
			getline(inFile, line);

			if (line.length() > 3) {

				if (line.at(0) == 'v') {
					if (line.at(1) == 'n') {
						readNormal(line);
					}
					else if (line.at(1) == ' ') {
						readVertex(line);
					}
					continue;
				}

				if (line.at(0) == 'f' && line.at(1) == ' ') {
					readFace(line);
				}
			}

		}
		inFile.close();
	}
}

void ObjReader::readVertex(std::string line) {
	float v1;
	float v2;
	float v3;

	sscanf(line.c_str(), "v %f %f %f", &v1, &v2, &v3);

	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

}

void ObjReader::readNormal(std::string line) {
	float v1;
	float v2;
	float v3;

	sscanf(line.c_str(), "vn %f %f %f", &v1, &v2, &v3);

	normals.push_back(v1);
	normals.push_back(v2);
	normals.push_back(v3);
}

void ObjReader::readFace(std::string line) {
	unsigned int v1;
	unsigned int n1;
	unsigned int v2;
	unsigned int n2;
	unsigned int v3;
	unsigned int n3;

	sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);

	faces.push_back(v1 - 1);
	//faces.push_back(n1);
	faces.push_back(v2 - 1);
	//faces.push_back(n2);
	faces.push_back(v3 - 1);
	//faces.push_back(n3);
}