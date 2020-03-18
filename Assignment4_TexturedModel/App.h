/**
 * The primary application code.
 */

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

class App : public QMainWindow
{
  Q_OBJECT

public:
  App(std::string objF, QWidget* parent=0);
  void setFile(std::string objFile);
  virtual ~App();
  
signals:

public slots:

private:
  std::string objFile;
  void buildGui();
};
