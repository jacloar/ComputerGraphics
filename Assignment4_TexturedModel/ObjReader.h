#pragma once
#include "QVector.h"
#include "QVector3D.h"
#include "QVector2D.h"

class ObjReader {

public:
	ObjReader();
	ObjReader(std::string fileName);

	QVector<QVector3D> vertices;
	QVector<QVector3D> normals;
	QVector<QVector2D> textures;

	//represents faces (indices of vertex indices and texture indices
	QVector<unsigned int> vertexIndices;
	QVector<unsigned int> textureIndices;

	QVector<QVector3D> getVertices();
	QVector<unsigned int> getFaces();
	~ObjReader();

private:
	void readFile(std::string fileName);
	void readVertex(std::string line);
	void readNormal(std::string line);
	void readFace(std::string line);
	void readTexture(std::string line);
};