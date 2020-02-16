#include "BasicWidget.h"
#include "ObjReader.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
    vbo_.release();
    vbo_.destroy();
    ibo_.release();
    ibo_.destroy();
    vao_.release();
    vao_.destroy();
}

//////////////////////////////////////////////////////////////////////
// Privates
QString BasicWidget::vertexShaderString() const
{
    QString str =
        "#version 330\n"
        "layout(location = 0) in vec3 position;\n"

        "uniform mat4 modelMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"

        "void main()\n"
        "{\n"
        "  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);\n"
        "}\n";
    return str;
}

QString BasicWidget::fragmentShaderString() const
{
    QString str =
        "#version 330\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "  color = vec4(0.5f, 1.0f, 0.25f, 1.f);\n"
        "}\n";
    return str;
}

void BasicWidget::createShader()
{
    QOpenGLShader vert(QOpenGLShader::Vertex);
    vert.compileSourceCode(vertexShaderString());
    QOpenGLShader frag(QOpenGLShader::Fragment);
    frag.compileSourceCode(fragmentShaderString());
    bool ok = shaderProgram_.addShader(&vert);
    if (!ok) {
        qDebug() << shaderProgram_.log();
    }
    ok = shaderProgram_.addShader(&frag);
    if (!ok) {
        qDebug() << shaderProgram_.log();
    }
    ok = shaderProgram_.link();
    if (!ok) {
        qDebug() << shaderProgram_.log();
    }
}

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // TODO
  // Handle key events here.
  qDebug() << "You Pressed an unsupported Key!";
  // ENDTODO
}

void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  QOpenGLContext* curContext = this->context();
  qDebug() << "[BasicWidget]::initializeGL() -- Context Information:";
  qDebug() << "  Context Valid: " << std::string(curContext->isValid() ? "true" : "false").c_str();
  qDebug() << "  GL Version Used: " << curContext->format().majorVersion() << "." << curContext->format().minorVersion();
  qDebug() << "  Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  qDebug() << "  Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  qDebug() << "  Version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << "  GLSL Version: " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

  ObjReader obj("../objects/cube.obj");

  // Set up our shaders.
  createShader();

  shaderProgram_.bind();
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.create();
  // Bind our vbo inside our vao
  vbo_.bind();
  vbo_.allocate(obj.vertices.data(), obj.vertices.size() * sizeof(GL_FLOAT));

  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ibo_.create();
  ibo_.bind();
  ibo_.allocate(obj.faces.data(), obj.faces.size() * sizeof(GL_INT));

  vao_.create();
  vao_.bind();
  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);

  vao_.release();
  vbo_.release();
  shaderProgram_.release();

  glViewport(0, 0, width(), height());
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);

  QMatrix4x4 modelMatrix_;
  modelMatrix_.scale(-0.5);

  QMatrix4x4 viewMatrix_;
  viewMatrix_.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

  QMatrix4x4 projectionMatrix_;
  projectionMatrix_.perspective(30., 16.0 / 9, 0.001, 1000.);

  shaderProgram_.bind();
  shaderProgram_.setUniformValue("modelMatrix", modelMatrix_);
  shaderProgram_.setUniformValue("viewMatrix", viewMatrix_);
  shaderProgram_.setUniformValue("projectionMatrix", projectionMatrix_);
}

void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram_.bind();
  vao_.bind();
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
  vao_.release();
  shaderProgram_.release();
}
