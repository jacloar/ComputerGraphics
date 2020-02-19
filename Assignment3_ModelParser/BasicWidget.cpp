#include "BasicWidget.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent), b_vbo_(QOpenGLBuffer::VertexBuffer), b_ibo_(QOpenGLBuffer::IndexBuffer), m_vbo_(QOpenGLBuffer::VertexBuffer), m_ibo_(QOpenGLBuffer::IndexBuffer)
{
    setFocusPolicy(Qt::StrongFocus);
    
}

BasicWidget::~BasicWidget()
{
    b_vbo_.release();
    b_vbo_.destroy();
    b_ibo_.release();
    b_ibo_.destroy();
    m_vbo_.release();
    m_vbo_.destroy();
    m_ibo_.release();
    m_ibo_.destroy();
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
        "layout(location = 1) in vec4 color;\n"
        "out vec4 vertColor;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(position, 1.0);\n"
        "  vertColor = color;\n"
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
        "  color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
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
    // Handle key events here.
    if (keyEvent->key() == Qt::Key_1) {
        showBunny = true;
        update();
    }
    else if (keyEvent->key() == Qt::Key_2) {
        showBunny = false;
        update();
    }
    else if (keyEvent->key() == Qt::Key_Q) {
        exit(0);
    }
    else {
        qDebug() << "You Pressed an unsupported Key!";
    }
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

    // Set up our shaders.
    createShader();

    // Set up our buffers and our vao
    // Temporary bind of our shader.
    shaderProgram_.bind();
    // Create and prepare a vbo
    b_vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    b_vbo_.create();
    b_vbo_.bind();
    b_vbo_.allocate(bunny.getVertices().data(), bunny.getVertices().size() * sizeof(GL_FLOAT));

    b_ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    b_ibo_.create();
    b_ibo_.bind();
    b_ibo_.allocate(bunny.getFaces().data(), bunny.getFaces().size() * sizeof(GL_INT));

    m_vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo_.create();
    m_vbo_.bind();
    m_vbo_.allocate(monkey.getVertices().data(), monkey.getVertices().size() * sizeof(GL_FLOAT));

    m_ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ibo_.create();
    m_ibo_.bind();
    m_ibo_.allocate(monkey.getFaces().data(), monkey.getFaces().size() * sizeof(GL_INT));

    // Create a VAO to keep track of things for us.
    vao_.create();
    vao_.bind();
    b_vbo_.bind();
    shaderProgram_.enableAttributeArray(0);
    shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    b_ibo_.bind();
    // Releae the vao THEN the vbo
    vao_.release();
    shaderProgram_.release();
    glViewport(0, 0, width(), height());
}

void BasicWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram_.bind();
    vao_.bind();

    if (showBunny) {
        b_vbo_.bind();
        shaderProgram_.enableAttributeArray(0);
        shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);

        b_ibo_.bind();

        glDrawElements(GL_TRIANGLES, bunny.getFaces().size(), GL_UNSIGNED_INT, 0);
    }
    else {
        m_vbo_.bind();
        shaderProgram_.enableAttributeArray(0);
        shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);

        m_ibo_.bind();

        glDrawElements(GL_TRIANGLES, monkey.getFaces().size(), GL_UNSIGNED_INT, 0);
    }
    
    vao_.release();
    shaderProgram_.release();
}
