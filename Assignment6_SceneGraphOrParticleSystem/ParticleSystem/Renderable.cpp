#include "Renderable.h"

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), numTris_(0), vertexSize_(0), rotationAxis_(0.0, 0.0, 1.0), rotationSpeed_(0.125)
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
		"in vec2 textureCoords;\n"

		"uniform mat4 modelMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"uniform mat4 projectionMatrix;\n"

		"out vec2 texCoords;\n"

		"void main()\n"
		"{\n"
		"  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);\n"
		"}\n";
	return str;
}

QString Renderable::fragmentShaderString() const
{
	QString str =
		"#version 330\n"
		"out vec4 fragColor;\n"

		"uniform sampler2D tex;\n"

		"void main()\n"
		"{\n"
		"  fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
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

	const QVector<QVector3D>& positions = obj.vertices;
	const QVector<QVector3D>& normals = obj.normals;
	
	//represent faces
	const QVector<unsigned int>& indexes = obj.vertexIndexes;

	this->obj = obj;

	// Set our model matrix to identity
	modelMatrix_.setToIdentity();

	vertexSize_ = 3;

	// Setup our shader.
	createShaders();

	// Now we can set up our buffers.
	// The VBO is created -- now we must create our VAO
	vao_.create();
	vao_.bind();
	vbo_.create();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.bind();

	vbo_.allocate(obj.data.data(), obj.data.size() * sizeof(float));

	// Create our index buffer
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);

	ibo_.allocate(obj.indexes.data(), obj.indexes.size() * sizeof(unsigned int));

	shader_.bind();

	// Make sure we setup our shader inputs properly
	shader_.enableAttributeArray(0);
	shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));

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

void Renderable::draw(const QMatrix4x4& transform, const QMatrix4x4& view, const QMatrix4x4& projection)
{
	// Make sure our state is what we want
	shader_.bind();
	// Set our matrix uniforms!
	QMatrix4x4 id;
	id.setToIdentity();
	shader_.setUniformValue("modelMatrix", transform);
	shader_.setUniformValue("viewMatrix", view);
	shader_.setUniformValue("projectionMatrix", projection);

	vao_.bind();
	glDrawElements(GL_TRIANGLES, obj.indexes.size(), GL_UNSIGNED_INT, 0);
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