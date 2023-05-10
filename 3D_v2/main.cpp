#include "QApplication"
#include "QCoreApplication"
#include "QSettings"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  QCoreApplication::setOrganizationName("mirrifer");
  QCoreApplication::setApplicationName("3D2");
  return a.exec();
}
