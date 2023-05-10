#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      openGL_(&control_),
      control_(&model_) {
  ui->setupUi(this);
  ui->stop->setDisabled(true);

  setWindowTitle("3DViewer v2.0");

  this->setFixedWidth(1500);
  this->setFixedHeight(1300);

  openGL_.setParent(ui->widget);

  allConnects();
  setSettinngs();
}

MainWindow::~MainWindow() {
  saveSet();
  delete ui;
}

void MainWindow::onFile() {
  QString f_path = QFileDialog::getOpenFileName(this, tr("Open File"), "/",
                                                tr("Object file (*.obj)"));

  QFileInfo inf(f_path);

  if (!f_path.isEmpty()) {
    ui->file_inf->clear();
  }

  if (!f_path.isEmpty()) {
    reset();
    openGL_.resetCameraRotate();
    try {
      control_.readFile(f_path.toStdString().c_str());
      file_open_ = true;
    } catch (const std::invalid_argument &ex) {
      errMsg(ex);
      file_open_ = false;
    }
    setFileInfo(file_open_, inf.fileName());
  }
}

void MainWindow::resetModel() {
  reset();
  control_.resetModifications();
  openGL_.resetCameraRotate();
}

void MainWindow::resetSettings() {
  QSettings settings("mirrifer", "3D2");
  settings.clear();
  setSettinngs();
}

void MainWindow::reset() {
  ui->moveX->setValue(0);
  ui->moveY->setValue(0);
  ui->moveZ->setValue(0);

  ui->rotateX->setValue(0);
  ui->rotateY->setValue(0);
  ui->rotateZ->setValue(0);

  ui->scaleXYZ->setValue(1);
}

void MainWindow::setFileInfo(bool type, QString name) {
  std::size_t col_ver = type ? control_.getCountOfCordinates() / 3 : 0;
  std::size_t col_reb = type ? control_.getCountOfFaces() +
                                   (control_.getCountOfCordinates() / 3) - 2
                             : 0;

  ui->file_inf->appendPlainText("File Name: " + name);
  ui->file_inf->appendPlainText("Kol-vo Vershin: " + QString::number(col_ver));
  ui->file_inf->appendPlainText("Kol-vo Reber: " + QString::number(col_reb));
}

void MainWindow::errMsg(const std::invalid_argument &ex) {
  QMessageBox er_msg;

  er_msg.setText(ex.what());
  er_msg.exec();
}

void MainWindow::lineType() {
  if (ui->spline->isChecked())
    openGL_.setLineType(0);
  else
    openGL_.setLineType(1);
}

void MainWindow::vertexType() {
  if (ui->otsut->isChecked())
    openGL_.setVertexType(0);
  else if (ui->krug->isChecked())
    openGL_.setVertexType(1);
  else
    openGL_.setVertexType(2);
}

void MainWindow::saveScreen() {
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...", "name", " BMP (*.bmp);; JPEG (*.jpg *.jpeg)");

  if (!file.isEmpty()) ui->widget->grab().save(file);
}

void MainWindow::createGif() {
  gif_ = new QGifImage();
  timer_ = new QTimer(this);

  ui->stop->setDisabled(false);

  connect(timer_, SIGNAL(timeout()), this, SLOT(addFrameToGif()));

  timer_->start(100);
}

void MainWindow::addFrameToGif() {
  gif_->addFrame(ui->widget->grab().toImage().scaled(
                     640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation),
                 0);

  count_of_frames_++;

  if (count_of_frames_ >= 50) stopGif();
}

void MainWindow::stopGif() {
  timer_->stop();
  delete timer_;
  QString file =
      QFileDialog::getSaveFileName(this, "Save as...", "name", " GIF (*.gif)");
  if (!file.isEmpty()) gif_->save(file);

  ui->stop->setDisabled(true);
  count_of_frames_ = 0;
  delete gif_;
  timer_ = nullptr;
  gif_ = nullptr;
}

void MainWindow::projection() {
  if (ui->parall->isChecked())
    openGL_.setProjectionType(0);
  else
    openGL_.setProjectionType(1);
}

void MainWindow::scaleIncrease(double v) {
  if (reset_scale_ != 0) control_.setScale(reset_scale_);
  if (v != 0) {
    reset_scale_ = 1 / v;
    control_.setScale(v);
    openGL_.sign();
  }
}

void MainWindow::rotateCord(double v, int p) {
  if (p == X) {
    if (reset_rotate_[X] != 0) control_.rotateByX(reset_rotate_[X]);
    reset_rotate_[X] = -v;
    control_.rotateByX(v);
  } else if (p == Y) {
    if (reset_rotate_[Y] != 0) control_.rotateByY(reset_rotate_[Y]);
    reset_rotate_[Y] = -v;
    control_.rotateByY(v);
  } else {
    if (reset_rotate_[Z] != 0) control_.rotateByZ(reset_rotate_[Z]);
    reset_rotate_[Z] = -v;
    control_.rotateByZ(v);
  }

  openGL_.sign();
}

void MainWindow::rotateX(double v) { rotateCord(v, X); }

void MainWindow::rotateY(double v) { rotateCord(v, Y); }

void MainWindow::rotateZ(double v) { rotateCord(v, Z); }

void MainWindow::moveCord(double v, int p) {
  if (reset_move_[p] != 0) control_.moveByXyz(reset_move_[p], p);
  reset_move_[p] = -v;
  control_.moveByXyz(v, p);
  openGL_.sign();
}

void MainWindow::moveX(double v) { moveCord(v, X); }

void MainWindow::moveY(double v) { moveCord(v, Y); }

void MainWindow::moveZ(double v) { moveCord(v, Z); }

void MainWindow::lineColorRedChanged(int v) { openGL_.setLineColor(v, 0); }

void MainWindow::lineColorGreenChanged(int v) { openGL_.setLineColor(v, 1); }

void MainWindow::lineColorBlueChanged(int v) { openGL_.setLineColor(v, 2); }

void MainWindow::lineWidthChanged(double v) { openGL_.setLineWidth(v); }

void MainWindow::vertexColorRedChanged(int v) { openGL_.setVertexColor(v, 0); }

void MainWindow::vertexColorGreenChanged(int v) {
  openGL_.setVertexColor(v, 1);
}

void MainWindow::vertexColorBlueChanged(int v) { openGL_.setVertexColor(v, 2); }

void MainWindow::vertexSizeChanged(double v) { openGL_.setVertexSize(v); }

void MainWindow::backColorRedChanged(int v) { openGL_.setBackColor(v, 0); }

void MainWindow::backColorGreenChanged(int v) { openGL_.setBackColor(v, 1); }

void MainWindow::backColorBlueChanged(int v) { openGL_.setBackColor(v, 2); }

void MainWindow::setSettinngs() {
  QSettings settings("mirrifer", "3D2");

  settings.beginGroup("PROJECTION");
  settings.value("Type", 1).toInt() == 0 ? ui->parall->click()
                                         : ui->central->click();
  settings.endGroup();

  settings.beginGroup("LINE");
  settings.value("Type", 0).toInt() == 0 ? ui->spline->click()
                                         : ui->stline->click();
  ui->clr_lr->setValue(settings.value("color_r", 255).toDouble());
  ui->clr_lg->setValue(settings.value("color_g", 255).toDouble());
  ui->clr_lb->setValue(settings.value("color_b", 255).toDouble());
  ui->width_l->setValue(settings.value("width", 1).toDouble());
  settings.endGroup();

  settings.beginGroup("VERTEX");
  int t = settings.value("Type", 0).toInt();
  t == 0   ? ui->otsut->click()
  : t == 1 ? ui->krug->click()
           : ui->kvadrat->click();
  ui->clr_vr->setValue(settings.value("color_r", 255).toDouble());
  ui->clr_vg->setValue(settings.value("color_g", 255).toDouble());
  ui->clr_vb->setValue(settings.value("color_b", 255).toDouble());
  ui->sz_v->setValue(settings.value("size", 2).toDouble());
  settings.endGroup();

  settings.beginGroup("BACK");
  ui->bkground_r->setValue(settings.value("color_r", 40).toDouble());
  ui->bkground_g->setValue(settings.value("color_g", 40).toDouble());
  ui->bkground_b->setValue(settings.value("color_b", 40).toDouble());
  settings.endGroup();
}

void MainWindow::saveSet() {
  QSettings settings("mirrifer", "3D2");

  settings.beginGroup("PROJECTION");
  settings.setValue("Type", openGL_.getProjectionType());
  settings.endGroup();

  settings.beginGroup("LINE");
  settings.setValue("Type", openGL_.getLineType());
  settings.setValue("color_r", openGL_.getLineColor()[0]);
  settings.setValue("color_g", openGL_.getLineColor()[1]);
  settings.setValue("color_b", openGL_.getLineColor()[2]);
  settings.setValue("width", openGL_.getLineWidth());
  settings.endGroup();

  settings.beginGroup("VERTEX");
  settings.setValue("Type", openGL_.getVertexType());
  settings.setValue("color_r", openGL_.getVertexColor()[0]);
  settings.setValue("color_g", openGL_.getVertexColor()[1]);
  settings.setValue("color_b", openGL_.getVertexColor()[2]);
  settings.setValue("size", openGL_.getVertexSize());
  settings.endGroup();

  settings.beginGroup("BACK");
  settings.setValue("color_r", openGL_.getBackColro()[0]);
  settings.setValue("color_g", openGL_.getBackColro()[1]);
  settings.setValue("color_b", openGL_.getBackColro()[2]);
  settings.endGroup();
}

void MainWindow::allConnects() {
  connect(ui->file, SIGNAL(clicked()), this, SLOT(onFile()));
  connect(ui->GroupLine, SIGNAL(idClicked(int)), this, SLOT(lineType()));
  connect(ui->GroupVertex, SIGNAL(idClicked(int)), this, SLOT(vertexType()));
  connect(ui->GroupProjection, SIGNAL(idClicked(int)), this,
          SLOT(projection()));
  connect(ui->save_screen, SIGNAL(clicked()), this, SLOT(saveScreen()));
  connect(ui->cr_gif, SIGNAL(clicked()), this, SLOT(createGif()));
  connect(ui->stop, SIGNAL(clicked()), this, SLOT(stopGif()));
  connect(ui->reset, SIGNAL(clicked()), this, SLOT(resetModel()));
  connect(ui->settings, SIGNAL(clicked()), this, SLOT(resetSettings()));

  connect(ui->scaleXYZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::scaleIncrease);
  connect(ui->rotateX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::rotateX);
  connect(ui->rotateY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::rotateY);
  connect(ui->rotateZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::rotateZ);

  connect(ui->moveX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::moveX);
  connect(ui->moveY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::moveY);
  connect(ui->moveZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::moveZ);

  connect(ui->clr_lr, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::lineColorRedChanged);
  connect(ui->clr_lg, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::lineColorGreenChanged);
  connect(ui->clr_lb, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::lineColorBlueChanged);
  connect(ui->width_l, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::lineWidthChanged);

  connect(ui->clr_vr, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::vertexColorRedChanged);
  connect(ui->clr_vg, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::vertexColorGreenChanged);
  connect(ui->clr_vb, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::vertexColorBlueChanged);
  connect(ui->sz_v, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::vertexSizeChanged);

  connect(ui->bkground_r, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::backColorRedChanged);
  connect(ui->bkground_g, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::backColorGreenChanged);
  connect(ui->bkground_b, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::backColorBlueChanged);
}
