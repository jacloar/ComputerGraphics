#include "BasicWidget.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(std::string of, QWidget* parent) : QOpenGLWidget(parent), logger_(this)
{

   objFile = of;
   //std::cout << "In BasicWidget constructor, objfile = " << objFile << std::endl;
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
    for (auto renderable : renderables_) {
        delete renderable;
    }
    renderables_.clear();
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
  } else if (keyEvent->key() == Qt::Key_Right) {
    qDebug() << "Right Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  }
  else if (keyEvent->key() == Qt::Key_Q) {
      qDebug() << "pressed q, exiting";
      exit(0);
      update();
  } else if (keyEvent->key() == Qt::Key_W) {
      qDebug() << "pressed w, switch wireframe mode";
      showWire = !showWire;
      update();
  } else {
    qDebug() << "You Pressed an unsupported Key!";
  }
}
void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  qDebug() << QDir::currentPath();

  //ObjReader obj("../../objects/house/house_obj.obj");
  ObjReader obj(objFile);

  //std::cout << "BW initializeGL" << objFile << std::endl;

  QVector<QVector3D> pos = obj.vertices;
  QVector<QVector3D> norm = obj.normals;
  QVector<QVector2D> texCoord = obj.textures;
  QVector<unsigned int> idx = obj.vertexIndexes;

  
  //QVector<unsigned int> t = obj.textureIndices;

  Renderable* ren = new Renderable();

  const QString qs (obj.ppmFile.c_str());
  ren->init(obj, qs);
  ren->setRotationAxis(QVector3D(0., 1., 0.));

  renderables_.push_back(ren);
  glViewport(0, 0, width(), height());
  frameTimer_.start();
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
  projection_.perspective(70.f, (float)w/(float)h, 0.001, 1000.0);

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

  for (auto renderable : renderables_) {
      renderable->update(msSinceRestart);
      renderable->draw(view_, projection_);
  }
  update();
}
