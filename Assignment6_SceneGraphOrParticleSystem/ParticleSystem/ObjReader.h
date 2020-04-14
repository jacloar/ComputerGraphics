#pragma once
#include <QtGui>
#include <QtOpenGL>

class ObjReader {

public:

	std::string ppmFile;

	ObjReader();
	ObjReader(std::string fileName);

	QVector<QVector3D> vertices;
	QVector<QVector3D> normals;
	QVector<QVector2D> textures;

	//represents faces (indices of vertex indices and texture indices
	QVector<unsigned int> vertexIndexes;
	QVector<unsigned int> textureIndexes;

	QVector<float> data;
	QVector<unsigned int> indexes;

	QVector<QVector3D> getVertices();
	QVector<unsigned int> getFaces();
	~ObjReader();

private:

	void readMtl(std::string line, std::string fileName);

	void readFile(std::string fileName);
	void readVertex(std::string line);
	void readNormal(std::string line);
	void readFace(std::string line);
	void readTexture(std::string line);
};