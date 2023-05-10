#ifndef VIEWER_V2_SRC_CONTROLLER_H
#define VIEWER_V2_SRC_CONTROLLER_H

#include "AdapterModel.h"

namespace s21 {

class Controller {
 public:
  Controller(AdapterModel* model);
  ~Controller();

  void readFile(const char* file_name);
  void moveByXyz(float value, int index) noexcept;
  void rotateByX(float value) noexcept;
  void rotateByY(float value) noexcept;
  void rotateByZ(float value) noexcept;
  void setScale(float scale) noexcept;

  std::size_t getCountOfFaces() noexcept;
  std::size_t getCountOfCordinates() noexcept;
  std::size_t getCountOfIndexes() noexcept;

  float getMaxAbsCordinateValue() noexcept;
  const float* getVertexArray() const noexcept;

  const unsigned int* getIndexArray() const noexcept;

  void resetModifications() noexcept;

 private:
  AdapterModel* model_;
};

}  // namespace s21

#endif  // VIEWER_V2_SRC_CONTROLLER_H
