#include "Renderable.h"

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <QOpenGLFunctions_3_3_core>
#include "qopenglext.h"
#include <iostream>
#include <GL/gl.h>

#define GL_GLEXT_LEGACY

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), normalMap(QOpenGLTexture::Target2D), numTris_(0), vertexSize_(0), rotationAxis_(0.0, 0.0, 1.0), rotationSpeed_(0.25)
{
	rotationAngle_ = 0.0;
}

Renderable::~Renderable()
{
	if (texture_.isCreated()) {
		texture_.destroy();
	}
	if (normalMap.isCreated()) {
		normalMap.destroy();
	}

	if (vbo_.isCreated()) {
		vbo_.destroy();
	}
	if (ibo_.isCreated()) {
		ibo_.destroy();
	}
	if (vao_.isCreated()) {
		vao_.destroy();
	}
}

void Renderable::createShaders()
{
	QString vertexFilename = "../../vert.glsl";
	bool ok = shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFilename);
	if (!ok) {
		qDebug() << shader_.log();
	}
	QString fragmentFilename = "../../frag.glsl";
	ok = shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFilename);
	if (!ok) {
		qDebug() << shader_.log();
	}
	ok = shader_.link();
	if (!ok) {
		qDebug() << shader_.log();
	}
}

void Renderable::init(const QVector<QVector3D>& positions, const QVector<QVector3D>& normals, const QVector<QVector2D>& texCoords, const QVector<unsigned int>& indexes, const QVector<QVector3D> tangents, const QVector<QVector3D> bitangents,const QString& textureFile, const QString& normalFile)
{
	// NOTE:  We do not currently do anything with normals -- we just
	// have it here for a later implementation!
	// We need to make sure our sizes all work out ok.
	

	//load textures
	texture_.setData(QImage(textureFile).mirrored(true, true));
	normalMap.setData(QImage(normalFile).mirrored(true, true));


	// Set our model matrix to identity
	modelMatrix_.setToIdentity();

	// set our number of trianges.
	numTris_ = indexes.size() / 3;

	// num verts (used to size our vbo)
	int numVerts = positions.size();
	vertexSize_ = 3 + 3 + 2 + 3 + 3;  // Position + normal + texCoord + tangents + bitangents
	int numVBOEntries = numVerts * vertexSize_;

	// Setup our shader.
	createShaders();

	// Now we can set up our buffers.
	// The VBO is created -- now we must create our VAO
	vao_.create();
	vao_.bind();
	vbo_.create();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.bind();
	// Create a temporary data array
	float* data = new float[numVBOEntries];
	for (int i = 0; i < numVerts; ++i) {
		data[i * vertexSize_ + 0] = positions.at(i).x();
		data[i * vertexSize_ + 1] = positions.at(i).y();
		data[i * vertexSize_ + 2] = positions.at(i).z();

		data[i * vertexSize_ + 3] = normals.at(i).x();
		data[i * vertexSize_ + 4] = normals.at(i).y();
		data[i * vertexSize_ + 5] = normals.at(i).z();

		data[i * vertexSize_ + 6] = texCoords.at(i).x();
		data[i * vertexSize_ + 7] = texCoords.at(i).y();

		data[i * vertexSize_ + 8] = tangents.at(i).x();
		data[i * vertexSize_ + 9] = tangents.at(i).y();
		data[i * vertexSize_ + 10] = tangents.at(i).z();

		data[i * vertexSize_ + 11] = bitangents.at(i).x();
		data[i * vertexSize_ + 12] = bitangents.at(i).y();
		data[i * vertexSize_ + 13] = bitangents.at(i).z();

	}
	vbo_.allocate(data, numVBOEntries * sizeof(float));
	delete[] data;

	// Create our index buffer
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// create a temporary array for our indexes
	unsigned int* idxAr = new unsigned int[indexes.size()];
	for (int i = 0; i < indexes.size(); ++i) {
		idxAr[i] = indexes.at(i);
	}

	ibo_.allocate(idxAr, indexes.size() * sizeof(unsigned int));
	delete[] idxAr;

	//position
	shader_.enableAttributeArray(0);
	shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));

	//normals
	shader_.enableAttributeArray(1);
	shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, vertexSize_ * sizeof(float));

	//text
	shader_.enableAttributeArray(2);
	shader_.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 2, vertexSize_ * sizeof(float));

	//tangent
	shader_.enableAttributeArray(3);
	shader_.setAttributeBuffer(3, GL_FLOAT, 8 * sizeof(float), 3, vertexSize_ * sizeof(float));

	//bitan
	shader_.enableAttributeArray(4);
	shader_.setAttributeBuffer(4, GL_FLOAT, 11 * sizeof(float), 3, vertexSize_ * sizeof(float));

	// Release our vao and THEN release our buffers.
	vao_.release();
	vbo_.release();
	ibo_.release();

}

void Renderable::update(const qint64 msSinceLastFrame)
{
	// For this lab, we want our polygon to rotate. 
	float sec = msSinceLastFrame / 1000.0f;
	float anglePart = sec * rotationSpeed_ * 360.f;
	rotationAngle_ += anglePart;
	while(rotationAngle_ >= 360.0) {
		rotationAngle_ -= 360.0;
	}
}

void Renderable::draw(const QMatrix4x4& world, const QMatrix4x4& view, const QMatrix4x4& projection)
{
	// Create our model matrix.
	QMatrix4x4 rotMatrix;
	rotMatrix.setToIdentity();
	rotMatrix.rotate(rotationAngle_, rotationAxis_);

	// incorporate a real world transform if want it.
	QMatrix4x4 modelMat = modelMatrix_ * rotMatrix;
	//modelMat = world * modelMat;

	shader_.bind();

	shader_.setUniformValue("modelMatrix", modelMat);
	shader_.setUniformValue("viewMatrix", view);
	shader_.setUniformValue("projectionMatrix", projection);

	shader_.setUniformValue("diffuseMap", 0);
	shader_.setUniformValue("normalMap", 1);

	shader_.setUniformValue("lightPos", QVector3D(0.0f, 0.0f, 0.0f));
	shader_.setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 0.0f));

	vao_.bind();

	texture_.bind(0);
	normalMap.bind(1);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	normalMap.release(1);
	texture_.release(0);
	
	
	vao_.release();
	shader_.release();

}

void Renderable::setModelMatrix(const QMatrix4x4& transform)
{
	modelMatrix_ = transform;
}

void Renderable::setRotationAxis(const QVector3D& axis)
{
	rotationAxis_ = axis;
}

void Renderable::setRotationSpeed(float speed)
{
	rotationSpeed_ = speed;
}

