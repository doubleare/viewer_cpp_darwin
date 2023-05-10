#include "AdapterModel.h"

s21::AdapterModel::AdapterModel() : initial_state_(nullptr) {
  model_ = &model_->getInstance();
}

s21::AdapterModel::~AdapterModel() { delete initial_state_; }

void s21::AdapterModel::openFile(const char *file_name) {
  model_->openFile(file_name);
  if (initial_state_) delete initial_state_;
  initial_state_ = model_->safeState();
}

std::size_t s21::AdapterModel::getCountOfCordinates() noexcept {
  return model_->getCountOfVertexes() * 3;
}

std::size_t s21::AdapterModel::getCountOfIndexes() noexcept {
  return model_->getCountOfIndexes();
}

std::size_t s21::AdapterModel::getCountOfFaces() noexcept {
  return model_->getCountOfFaces();
}

const float *s21::AdapterModel::getVertex() const noexcept {
  return model_->getVertex();
}

const unsigned int *s21::AdapterModel::getIndex() const noexcept {
  return model_->getIndex();
}

float s21::AdapterModel::getMaxCordinateVal() noexcept {
  return (model_->getMaxCordinateVal() + 2) * 1.5;
}

void s21::AdapterModel::toInitialState() { model_->undo(initial_state_); }

void s21::AdapterModel::scale(float s) noexcept { return model_->scale(s); }

void s21::AdapterModel::moveXyz(float val, int cordinate) noexcept {
  return model_->moveXyz(val, cordinate);
}

void s21::AdapterModel::rotateX(float val) noexcept {
  val = val * M_PI / 180;
  return model_->rotateX(val);
}

void s21::AdapterModel::rotateY(float val) noexcept {
  val = val * M_PI / 180;
  return model_->rotateY(val);
}

void s21::AdapterModel::rotateZ(float val) noexcept {
  val = val * M_PI / 180;
  return model_->rotateZ(val);
}
