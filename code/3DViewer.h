#ifndef VIEWER_V2_SRC_3DVIEWER_V2_H
#define VIEWER_V2_SRC_3DVIEWER_V2_H

#include <cmath>
#include <iostream>
#include <regex>

#include "Memento.h"

enum oses { X, Y, Z };

namespace s21 {

class SingltonViewer {
 public:
  ~SingltonViewer();

  static SingltonViewer& getInstance();

  void openFile(const char* file_name);
  std::size_t getCountOfVertexes() noexcept;
  std::size_t getCountOfIndexes() noexcept;
  std::size_t getCountOfFaces() noexcept;
  const float* getVertex() const noexcept;
  const unsigned int* getIndex() const noexcept;
  float getMaxCordinateVal() noexcept;
  void scale(float s) noexcept;
  void moveXyz(float val, int cordinate) noexcept;
  void rotateX(float val) noexcept;
  void rotateY(float val) noexcept;
  void rotateZ(float val) noexcept;

  Memento* safeState();
  void undo(Memento* state);

 private:
  FILE* opened_file_;
  std::size_t count_of_faces_, count_of_vertexes_, count_of_indexes_;
  float abs_max_;
  float* xyz_arr_;
  unsigned int* index_;
  bool first_file;

  SingltonViewer();

  void countFacesVertexes(FILE* file);
  void countIndexesInFace(char* str);
  void maxCordinateVal() noexcept;
  void fromFileToArrs();
  void indexesToArr(char* str, int* ind);
  void allocMem();
  void freeMem();
  void resetNumeric();
  void reset();
};

}  // namespace s21

#endif  // VIEWER_V2_SRC_3DVIEWER_V2_H