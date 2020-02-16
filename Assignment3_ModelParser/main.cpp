/**
 * Support code written by Erik W. Anderson
 */

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <stdio.h>
#include <iostream>


// Lab application
#include "Application.h"
#include "ObjReader.h"

int main(int argc, char** argv) {

  //ObjReader r("../objects/monkey.obj");
    
  QApplication a(argc, argv);
  QString appDir = a.applicationDirPath();
  QDir::setCurrent(appDir);

  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  fmt.setStencilBufferSize(8);
  fmt.setVersion(3,3);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(fmt);
  
  Application app;
  app.show();
  return QApplication::exec();
}
