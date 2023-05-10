#ifndef VIEWER_V2_SRC_MEMENTO_H
#define VIEWER_V2_SRC_MEMENTO_H

namespace s21 {

class SingltonViewer;

class Memento {
 public:
  Memento(float* arr, int size);
  ~Memento();

 private:
  friend class SingltonViewer;
  float* array_ = nullptr;
};

}  // namespace s21

#endif  // VIEWER_V2_SRC_MEMENTO_H