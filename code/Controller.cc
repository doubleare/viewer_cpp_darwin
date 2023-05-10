#include "Controller.h"

s21::Controller::Controller(AdapterModel *model) : model_(model) {}

s21::Controller::~Controller() { model_ = nullptr; }

void s21::Controller::readFile(const char *file_name) {
  model_->openFile(file_name);
}

void s21::Controller::moveByXyz(float value, int index) noexcept {
  model_->moveXyz(value, index);
}

void s21::Controller::rotateByX(float value) noexcept {
  model_->rotateX(value);
}

void s21::Controller::rotateByY(float value) noexcept {
  model_->rotateY(value);
}

void s21::Controller::rotateByZ(float value) noexcept {
  model_->rotateZ(value);
}

void s21::Controller::setScale(float scale) noexcept { model_->scale(scale); }

std::size_t s21::Controller::getCountOfFaces() noexcept {
  return model_->getCountOfFaces();
}

std::size_t s21::Controller::getCountOfCordinates() noexcept {
  return model_->getCountOfCordinates();
}

std::size_t s21::Controller::getCountOfIndexes() noexcept {
  return model_->getCountOfIndexes();
}

float s21::Controller::getMaxAbsCordinateValue() noexcept {
  return model_->getMaxCordinateVal();
}

const float *s21::Controller::getVertexArray() const noexcept {
  return model_->getVertex();
}

const unsigned int *s21::Controller::getIndexArray() const noexcept {
  return model_->getIndex();
}

void s21::Controller::resetModifications() noexcept {
  model_->toInitialState();
}
