#include "Memento.h"

s21::Memento::Memento(float *arr, int size) {
  array_ = new float[size]{};
  for (int i = 0; i < size; i++) {
    array_[i] = arr[i];
  }
}

s21::Memento::~Memento() {
  if (array_) {
    delete[] array_;
    array_ = nullptr;
  }
}
