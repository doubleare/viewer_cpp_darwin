#ifndef OGL_H
#define OGL_H
#define GL_SILENCE_DEPRECATION

#include "../code/Controller.h"
#include "QMouseEvent"
#include "QOpenGLWidget"

class OGL : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit OGL(s21::Controller *control, QWidget *parent = nullptr);

  void drawScene();
  void sign();

  void setLineType(int type);
  int getLineType();

  void setLineWidth(GLfloat type);
  GLfloat getLineWidth();

  void setLineColor(GLfloat colorRGB, int ind);
  const GLfloat *getLineColor() const;

  void setVertexType(int type);
  int getVertexType();

  void setVertexSize(GLfloat size);
  GLfloat getVertexSize();

  void setVertexColor(GLfloat colorRGB, int ind);
  const GLfloat *getVertexColor() const;

  void setBackColor(GLfloat colorRGB, int ind);
  const GLfloat *getBackColro() const;

  void setProjectionType(int type);
  int getProjectionType();

  void resetCameraRotate();

 private:
  s21::Controller *controller_;
  QPoint mouse_point_;
  GLuint vbo_, ibo_;
  GLfloat line_width_, vertox_size_;
  GLfloat x_rotate_, y_rotate_, prev_x_rotate_, prev_y_rotate_;
  GLfloat color_line_[3], color_vertex_[3], color_back_[3];

  int type_line_, type_vertex_, type_projection_;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *event) override;

  void initVbo();
};

#endif  // OGL_H
