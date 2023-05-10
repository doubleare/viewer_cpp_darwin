#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../code/Controller.h"
#include "QButtonGroup"
#include "QFileDialog"
#include "QFileInfo"
#include "QMainWindow"
#include "QMessageBox"
#include "QSettings"
#include "QTimer"
#include "ogl.h"
#include "qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void onFile();
  void lineType();
  void vertexType();
  void saveScreen();
  void createGif();
  void stopGif();
  void projection();
  void addFrameToGif();
  void resetModel();
  void resetSettings();

  void scaleIncrease(double v);
  void rotateX(double v);
  void rotateY(double v);
  void rotateZ(double v);
  void moveX(double v);
  void moveY(double v);
  void moveZ(double v);

  void lineColorRedChanged(int v);
  void lineColorGreenChanged(int v);
  void lineColorBlueChanged(int v);
  void lineWidthChanged(double v);

  void vertexColorRedChanged(int v);
  void vertexColorGreenChanged(int v);
  void vertexColorBlueChanged(int v);
  void vertexSizeChanged(double v);

  void backColorRedChanged(int v);
  void backColorGreenChanged(int v);
  void backColorBlueChanged(int v);

 private:
  Ui::MainWindow *ui;
  OGL openGL_;
  QGifImage *gif_;
  QTimer *timer_;
  s21::AdapterModel model_;
  s21::Controller control_;

  double reset_move_[3] = {0, 0, 0};
  double reset_rotate_[3] = {0, 0, 0};
  double reset_scale_ = 0;

  int count_of_frames_ = 0;

  bool file_open_ = false;

  void errMsg(const std::invalid_argument &ex);
  void setFileInfo(bool type, QString name);

  void rotateCord(double v, int p);
  void moveCord(double v, int p);
  void allConnects();

  void setSettinngs();
  void saveSet();
  void reset();
};
#endif  // MAINWINDOW_H
