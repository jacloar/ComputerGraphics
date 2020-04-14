#include "BasicWidget.h"
#include "Emitter.h"

#include <iostream>
#include <QtGui>
#include <QtOpenGL>

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(std::string of, QWidget* parent) : QOpenGLWidget(parent), logger_(this)
{

    objFile = of;
    setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
    delete emitter_;
    delete renderable_;
}

//////////////////////////////////////////////////////////////////////
// Privates
///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
    // Handle key events here.
    if (keyEvent->key() == Qt::Key_Left) {
        qDebug() << "Left Arrow Pressed";
        update();  // We call update after we handle a key press to trigger a redraw when we are ready
    }
    else if (keyEvent->key() == Qt::Key_Right) {
        qDebug() << "Right Arrow Pressed";
        update();  // We call update after we handle a key press to trigger a redraw when we are ready
    }
    else if (keyEvent->key() == Qt::Key_Q) {
        qDebug() << "pressed q, exiting";
        exit(0);
        update();
    }
    else if (keyEvent->key() == Qt::Key_W) {
        qDebug() << "pressed w, switch wireframe mode";
        showWire = !showWire;
        update();
    }
    else {
        qDebug() << "You Pressed an unsupported Key!";
    }
}
void BasicWidget::initializeGL()
{
    std::cout << "initializing basic widget\n";
    makeCurrent();
    initializeOpenGLFunctions();

    qDebug() << QDir::currentPath();

    ObjReader obj("../../objects/bunny.obj");

    QVector<QVector3D> pos = obj.vertices;
    QVector<QVector3D> norm = obj.normals;
    QVector<QVector2D> texCoord = obj.textures;
    QVector<unsigned int> idx = obj.vertexIndexes;

    renderable_ = new Renderable();

    const QString qs(obj.ppmFile.c_str());
    renderable_->init(obj, qs);

    emitter_ = new Emitter(QVector3D(0, 0, -8), QVector3D(0., 1., 0.), 20, renderable_);

    glViewport(0, 0, width(), height());
    frameTimer_.start();
    std::cout << "initialized basic widget\n";
}




void BasicWidget::resizeGL(int w, int h)
{
    if (!logger_.isLogging()) {
        logger_.initialize();
        // Setup the logger for real-time messaging
        connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=]() {
            const QList<QOpenGLDebugMessage> messages = logger_.loggedMessages();
            for (auto msg : messages) {
                qDebug() << msg;
            }
            });
        logger_.startLogging();
    }

    view_.setToIdentity();
    view_.lookAt(QVector3D(0.0f, 0.0f, 4.0f),
        QVector3D(0.0f, 0.0f, 0.0f),
        QVector3D(0.0f, 1.0f, 0.0f));

    projection_.setToIdentity();
    projection_.perspective(70.f, (float)w / (float)h, 0.001, 1000.0);

    glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
    qint64 msSinceRestart = frameTimer_.restart();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, showWire ? GL_LINE : GL_FILL);

    emitter_->update(msSinceRestart);
    emitter_->draw(view_, projection_);

    update();
}
