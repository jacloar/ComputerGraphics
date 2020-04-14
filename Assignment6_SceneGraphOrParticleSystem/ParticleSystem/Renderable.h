#pragma once

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "ObjReader.h"

class Renderable
{
protected:
	// Each renderable has its own model matrix
	QMatrix4x4 modelMatrix_;
	// For now, we have only one shader per object
	QOpenGLShaderProgram shader_;
	// For now, we have only one texture per object
	QOpenGLTexture texture_;
	// For now, we have a single unified buffer per object
	QOpenGLBuffer vbo_;
	// Make sure we have an index buffer.
	QOpenGLBuffer ibo_;
	// We have a single draw call, so a single vao
	QOpenGLVertexArrayObject vao_;
	// Keep track of how many triangles we actually have to draw in our ibo
	unsigned int numTris_;
	int vertexSize_;

	// Define our axis of rotation for animation
	QVector3D rotationAxis_;
	float rotationSpeed_;
	float rotationAngle_;

	// Create our shader and fix it up
	void createShaders();

	QString Renderable::vertexShaderString() const;
	QString Renderable::fragmentShaderString() const;

	ObjReader obj;

public:
	Renderable();
	virtual ~Renderable();

	// When we initialize our renderable, we pass it normals.  We 
	// currently don't use normals in our implementation, but the array is checked
	// for the appropriate size.  The values can be all 0, but must be the same size as
	// the position array!
	virtual void init(const ObjReader obj, const QString& textureFile);
	virtual void update(const qint64 msSinceLastFrame);
	virtual void draw(const QMatrix4x4& transform, const QMatrix4x4& view, const QMatrix4x4& projection);

	void setModelMatrix(const QMatrix4x4& transform);
	void setRotationAxis(const QVector3D& axis);
	void setRotationSpeed(float speed);

private:

};