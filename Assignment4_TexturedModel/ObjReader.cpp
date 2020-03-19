#include "ObjReader.h"
#include "qstring.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>


ObjReader::ObjReader() {}

ObjReader::ObjReader(std::string fileName) {
	readFile(fileName);

	QVector<QVector<float>> vecs;

	for (int i = 0; i < vertexIndexes.size(); ++i) {
		QVector3D vert = vertices.at(vertexIndexes.at(i));
		QVector2D tex = textures.at(textureIndexes.at(i));

		QVector<float> vec;
		vec.push_back(vert.x());
		vec.push_back(vert.y());
		vec.push_back(vert.z());
		vec.push_back(tex.x());
		vec.push_back(tex.y());

		int idx = vecs.indexOf(vec);
		if (idx == -1) {
			idx = vecs.size();
			vecs.push_back(vec);

			data.append(vec);
		}

		indexes.push_back(idx);
	}
}

QVector<QVector3D> ObjReader::getVertices() {
	return vertices;
}

QVector<unsigned int> ObjReader::getFaces() {
	return vertexIndexes;
}


ObjReader::~ObjReader() {

}


void ObjReader::readFile(std::string fileName) {
	std::ifstream inFile;
	inFile.open(fileName.c_str());

	bool seeanMtl = false;

	if (inFile.is_open()) {
		while (!inFile.eof()) {

			std::string line;
			getline(inFile, line);

			if (line.length() > 3) {

				//parse material file, first boolean for performance 
				if (!seeanMtl && line.substr(0, 6) == "mtllib") {
					readMtl(line, fileName);
				}
				

				if (line.at(0) == 'v') {
					if (line.at(1) == 'n') {
						readNormal(line);
					}
					else if (line.at(1) == 't') {
						readTexture(line);
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



void ObjReader::readMtl(std::string line, std::string fileName) {


	std::string s (fileName);

	s = s.substr(0, s.find_last_of("\\/")); //get directory of .obj file

//    std::cout << "reading mtl, path =  " << s << std::endl;

	std::string fullPath = s + "/" + line.substr(7, line.length()); //fulll file path of .mtl file
	//std::cout << "full mtl path = " << fullPath << std::endl;
	
	std::ifstream mtlFile; 
	mtlFile.open(fullPath);
	std::string gline;
	
   std::string ppmTemp = "";

	if (mtlFile.is_open()) {
		while (!mtlFile.eof()) {
			getline(mtlFile, gline);
			
			if (gline.substr(0, 6) == "map_Kd"){ 
				ppmTemp = gline.substr(7, gline.length());
				break;
			}

		}
	}

	ppmTemp = s + "/" + ppmTemp;
	ppmFile = ppmTemp;
	std::cout << "ppm file now = " << ppmFile << std::endl;
	mtlFile.close();
}

void ObjReader::readVertex(std::string line) {
	float v1;
	float v2;
	float v3;

	sscanf(line.c_str(), "v %f %f %f", &v1, &v2, &v3);

	vertices.push_back(QVector3D(v1, v2, v3));
}

void ObjReader::readNormal(std::string line) {
	float v1;
	float v2;
	float v3;

	sscanf(line.c_str(), "vn %f %f %f", &v1, &v2, &v3);

	normals.push_back(QVector3D(v1, v2, v3));
}

void ObjReader::readTexture(std::string line) {
	float t1;
	float t2;

	sscanf(line.c_str(), "vt %f %f", &t1, &t2);

	textures.push_back(QVector2D(t1, t2));
}

void ObjReader::readFace(std::string line) {
	//vertex one, two, and three
	unsigned int v1;
	unsigned int v2;
	unsigned int v3;

	//texture indicies one, two, three
	unsigned int t1;
	unsigned int t2;
	unsigned int t3;

	sscanf(line.c_str(), "f %d/%d/%*d %d/%d/%*d %d/%d/%*d", &v1, &t1, &v2, &t2, &v3, &t3);

	//try to find a similar ( = same fo all attributes ) vertex

	vertexIndexes.push_back(v1 - 1);
	vertexIndexes.push_back(v2 - 1);
	vertexIndexes.push_back(v3 - 1);
	
	textureIndexes.push_back(t1 - 1);
	textureIndexes.push_back(t2 - 1);
	textureIndexes.push_back(t3 - 1);
	
}