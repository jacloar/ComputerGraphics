#pragma once
#include "QVector.h"
#include "qvector.h"

class ObjReader {

public:
	ObjReader();
	ObjReader(std::string fileName);

	QVector<float> vertices;
	QVector<float> normals;
	QVector<float> textures;
	QVector<unsigned int> vertexIndices;
	QVector<unsigned int> textureIndices;

	QVector<float> getVertices();
	QVector<unsigned int> getFaces();
	~ObjReader();

private:
	void readFile(std::string fileName);
	void readVertex(std::string line);
	void readNormal(std::string line);
	void readFace(std::string line);
	void readTexture(std::string line);
};