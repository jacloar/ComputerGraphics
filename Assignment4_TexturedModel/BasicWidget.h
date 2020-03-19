#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>

#include "Renderable.h"
#include "ObjReader.h"

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
   
  boolean showWire = false; 


  QMatrix4x4 model_;
  QMatrix4x4 view_;
  QMatrix4x4 projection_;
  
  QElapsedTimer frameTimer_;

  QVector<Renderable*> renderables_;

  QOpenGLDebugLogger logger_;

protected:
    std::string objFile;
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  
public:
  BasicWidget(std::string of, QWidget* parent=nullptr);
  virtual ~BasicWidget();
  
  // Make sure we have some size that makes sense.
  QSize sizeHint() const {return QSize(800,600);}
};
