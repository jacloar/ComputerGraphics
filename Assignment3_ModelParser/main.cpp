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

    ObjReader r("../objects/cube.obj");
    

    /*
    std::cout << "vertices: \n";
    for (int i = 0; i < r.vertices.size(); i++) {
        std::cout << r.vertices.at(i) << std::endl;
    }

    std::cout << "\n normals: \n";
    for (int i = 0; i < r.normals.size(); i++) {
        std::cout << r.normals.at(i) << std::endl;
    }
    */

    std::cout << "\n faces: \n";
    for (int i = 0; i < r.faces.size(); i++) {
        std::cout << r.faces.at(i) << std::endl;
    }

  



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
