#include "App.h"

#include "BasicWidget.h"
#include <iostream>

App::App(std::string objF, QWidget* parent) : QMainWindow(parent)
{

  objFile = objF;
  buildGui();
}

App::~App()
{}



void App::buildGui()
{
  // A simple menubar.
  QMenuBar* menu = menuBar();
  QMenu* file = menu->addMenu("File");
  QAction* exit = file->addAction("Quit", [this]() {close();});


  //std::cout << "IN app, objfile = " << objFile << std::endl;
  

  // Our basic widget.
  BasicWidget* widget = new BasicWidget(objFile, this);
  
  setCentralWidget(widget);
}
