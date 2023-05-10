#ifndef VIEWER_V2_SRC_ADAPTERMODEL_V2_H
#define VIEWER_V2_SRC_ADAPTERMODEL_V2_H

#include "3DViewer.h"

namespace s21 {

class AdapterModel {
 public:
  AdapterModel();
  ~AdapterModel();

  void openFile(const char* file_name);
  std::size_t getCountOfCordinates() noexcept;
  std::size_t getCountOfIndexes() noexcept;
  std::size_t getCountOfFaces() noexcept;
  const float* getVertex() const noexcept;
  const unsigned int* getIndex() const noexcept;
  float getMaxCordinateVal() noexcept;
  void toInitialState();
  void scale(float s) noexcept;
  void moveXyz(float val, int cordinate) noexcept;
  void rotateX(float val) noexcept;
  void rotateY(float val) noexcept;
  void rotateZ(float val) noexcept;

 private:
  SingltonViewer* model_;
  Memento* initial_state_;
};

}  // namespace s21

#endif  // VIEWER_V2_SRC_ADAPTERMODEL_V2_H