#pragma once
#include "QVector.h"
#include "qvector.h"

class ObjReader {

public:
	ObjReader();
	ObjReader(std::string fileName);

	QVector<float> vertices;
	QVector<float> normals;
	QVector<unsigned int> faces;

	QVector<float> getVertices();
	QVector<unsigned int> getFaces();

private:
	void readFile(std::string fileName);
	void readVertex(std::string line);
	void readNormal(std::string line);
	void readFace(std::string line);
};