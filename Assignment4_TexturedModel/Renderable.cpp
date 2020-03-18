#include "Renderable.h"

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), numTris_(0), vertexSize_(0), rotationAxis_(0.0, 0.0, 1.0), rotationSpeed_(0.25)
{
	rotationAngle_ = 0.0;
}

Renderable::~Renderable()
{
	if (texture_.isCreated()) {
		texture_.destroy();
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

QString Renderable::vertexShaderString() const
{
	QString str =
		"#version 330\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec4 color;\n"

		"uniform mat4 modelMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"uniform mat4 projectionMatrix;\n"

		"out vec4 vertColor;\n"
		"void main()\n"
		"{\n"
		"  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);\n"
		"  vertColor = color;\n"
		"}\n";
	return str;
}

QString Renderable::fragmentShaderString() const
{
	QString str =
		"#version 330\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"  color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n";
	return str;
}

void Renderable::createShaders()
{
	QOpenGLShader vert(QOpenGLShader::Vertex);
	vert.compileSourceCode(vertexShaderString());
	QOpenGLShader frag(QOpenGLShader::Fragment);
	frag.compileSourceCode(fragmentShaderString());
	bool ok = shader_.addShader(&vert);
	if (!ok) {
		qDebug() << shader_.log();
	}
	ok = shader_.addShader(&frag);
	if (!ok) {
		qDebug() << shader_.log();
	}
	ok = shader_.link();
	if (!ok) {
		qDebug() << shader_.log();
	}
}

void Renderable::init(const ObjReader obj, const QString& textureFile)
{
	// NOTE:  We do not currently do anything with normals -- we just
	// have it here for a later implementation!
	// We need to make sure our sizes all work out ok.

	/*
	if (positions.size() != texCoords.size() ||
		positions.size() != normals.size()) {
		qDebug() << "[Renderable]::init() -- positions size mismatch with normals/texture coordinates";
		return;
	}*/

	const QVector<QVector3D>& positions = obj.vertices;
	const QVector<QVector3D>& normals = obj.normals;
	const QVector<QVector2D>& texCoords = obj.textures;
	
	
	//represent faces
	const QVector<unsigned int>& indexes = obj.vertexIndices;
	//const QVector<unsigned int> textureIndices = obj.textureIndices;


	this->obj = obj;


	// Set our model matrix to identity
	modelMatrix_.setToIdentity();

	// Load our texture.
	texture_.setData(QImage(textureFile));


	//number of triangles
	int numT = indexes.size() / 3;



	// num verts (used to size our vbo)
	int numVerts = positions.size();
	vertexSize_ = 3 + 2;  // Position + texCoord
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
		data[i * vertexSize_ + 3] = texCoords.at(i).x();
		data[i * vertexSize_ + 4] = texCoords.at(i).y();
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

	shader_.bind();

	// Make sure we setup our shader inputs properly
	shader_.enableAttributeArray(0);
	shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));
	shader_.enableAttributeArray(1);
	shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, vertexSize_ * sizeof(float));

	// Release our vao and THEN release our buffers.
	vao_.release();

	shader_.release();
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

void Renderable::draw(const QMatrix4x4& view, const QMatrix4x4& projection)
{
	// Create our model matrix.
	QMatrix4x4 rotMatrix;
	rotMatrix.setToIdentity();
	rotMatrix.rotate(rotationAngle_, rotationAxis_);

	QMatrix4x4 modelMat = modelMatrix_ * rotMatrix;
	// Make sure our state is what we want
	shader_.bind();
	// Set our matrix uniforms!
	QMatrix4x4 id;
	id.setToIdentity();
	shader_.setUniformValue("modelMatrix", modelMat);
	shader_.setUniformValue("viewMatrix", view);
	shader_.setUniformValue("projectionMatrix", projection);

	vao_.bind();
	texture_.bind();
	glDrawElements(GL_TRIANGLES, obj.textureIndices.size(), GL_UNSIGNED_INT, 0);
	texture_.release();
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