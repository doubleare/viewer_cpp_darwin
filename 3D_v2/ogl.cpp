#include "ogl.h"

OGL::OGL(s21::Controller *control, QWidget *parent)
    : QOpenGLWidget(parent), controller_(control) {}

void OGL::initializeGL() {
  setGeometry(0, 0, 1200, 1200);
  glEnable(GL_DEPTH_TEST);
}

void OGL::initVbo() {
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ibo_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

  glBufferData(GL_ARRAY_BUFFER,
               controller_->getCountOfCordinates() * sizeof(GLfloat),
               controller_->getVertexArray(), GL_STREAM_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               controller_->getCountOfIndexes() * sizeof(GLuint),
               controller_->getIndexArray(), GL_STATIC_DRAW);
}
void OGL::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OGL::paintGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLfloat max = controller_->getMaxAbsCordinateValue();

  if (type_projection_ == 0)
    glOrtho(-max, max, -max, max, 0, max * 100);
  else
    glFrustum(-1, 1, -1, 1, 1, max * 100);

  glClearColor(color_back_[0] / 255.0, color_back_[1] / 255.0,
               color_back_[2] / 255.0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (type_projection_ == 0)
    glTranslatef(0, 0, -((max)*2));
  else
    glTranslatef(0, 0, -(max));

  glRotatef(prev_x_rotate_ + x_rotate_, 1, 0, 0);
  glRotatef(prev_y_rotate_ + y_rotate_, 0, 1, 0);
  drawScene();
}

void OGL::mousePressEvent(QMouseEvent *m) {
  mouse_point_ = m->pos();
  prev_x_rotate_ += x_rotate_;
  prev_y_rotate_ += y_rotate_;
}

void OGL::mouseMoveEvent(QMouseEvent *m) {
  x_rotate_ = 1 / M_PI * (m->pos().y() - mouse_point_.y());
  y_rotate_ = 1 / M_PI * (m->pos().x() - mouse_point_.x());
  update();
}

void OGL::wheelEvent(QWheelEvent *event) {
  QPoint numDegrees = event->angleDelta();
  if (numDegrees.y() < 0) controller_->setScale(1 - 0.1);
  if (numDegrees.y() > 0) controller_->setScale(1 + 0.1);
  update();
}

void OGL::drawScene() {
  initVbo();

  glVertexPointer(3, GL_FLOAT, 0, (void *)0);
  glEnableClientState(GL_VERTEX_ARRAY);

  if (type_vertex_ != 0) {
    if (type_vertex_ == 1) glEnable(GL_POINT_SMOOTH);
    glColor3d(color_vertex_[0] / 255.0, color_vertex_[1] / 255.0,
              color_vertex_[2] / 255.0);
    glPointSize(vertox_size_);
    glDrawArrays(GL_POINTS, 0, controller_->getCountOfCordinates() / 3);
    if (type_vertex_ == 1) glDisable(GL_POINT_SMOOTH);
  }

  if (type_line_ == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x00FF);
  }
  glColor3d(color_line_[0] / 255.0, color_line_[1] / 255.0,
            color_line_[2] / 255.0);
  glLineWidth(line_width_);

  glDrawElements(GL_LINES, controller_->getCountOfIndexes(), GL_UNSIGNED_INT,
                 (void *)0);

  if (type_line_ == 1) glDisable(GL_LINE_STIPPLE);

  glDisableClientState(GL_VERTEX_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ibo_);
}

void OGL::sign() { update(); }

void OGL::setLineType(int type) {
  type_line_ = type;
  update();
}

void OGL::setLineWidth(GLfloat width) {
  line_width_ = width;
  update();
}

void OGL::setLineColor(GLfloat colorRGB, int ind) {
  color_line_[ind] = colorRGB;
  update();
}

void OGL::setVertexType(int type) {
  type_vertex_ = type;
  update();
}

void OGL::setVertexSize(GLfloat size) {
  vertox_size_ = size;
  update();
}

void OGL::setVertexColor(GLfloat colorRGB, int ind) {
  color_vertex_[ind] = colorRGB;
  update();
}

void OGL::setBackColor(GLfloat colorRGB, int ind) {
  color_back_[ind] = colorRGB;
  update();
}

void OGL::setProjectionType(int type) {
  type_projection_ = type;
  update();
}

int OGL::getLineType() { return type_line_; }

GLfloat OGL::getLineWidth() { return line_width_; }

const GLfloat *OGL::getLineColor() const { return color_line_; }

int OGL::getVertexType() { return type_vertex_; }

GLfloat OGL::getVertexSize() { return vertox_size_; }

const GLfloat *OGL::getVertexColor() const { return color_vertex_; }

const GLfloat *OGL::getBackColro() const { return color_back_; }

int OGL::getProjectionType() { return type_projection_; }

void OGL::resetCameraRotate() {
  x_rotate_ = y_rotate_ = 0;
  prev_x_rotate_ = prev_y_rotate_ = 0;
  update();
}
