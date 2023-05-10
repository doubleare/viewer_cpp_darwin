#include "3DViewer.h"

s21::SingltonViewer::SingltonViewer() : first_file(true) { resetNumeric(); }

s21::SingltonViewer::~SingltonViewer() {
  if (!first_file) reset();
}

s21::SingltonViewer &s21::SingltonViewer::getInstance() {
  static SingltonViewer instance;
  return instance;
}

void s21::SingltonViewer::openFile(const char *file_name) {
  if (std::regex_match(file_name,
                       std::regex("^.*(.obj)$", std::regex_constants::icase))) {
    FILE *tmp_file = fopen(file_name, "r");
    if (tmp_file != NULL) {
      first_file ? resetNumeric() : reset();
      countFacesVertexes(tmp_file);
      if (count_of_faces_ == 0 || count_of_vertexes_ == 0 ||
          count_of_indexes_ == 0) {
        resetNumeric();
        throw std::invalid_argument("invalid file");
      }
      allocMem();
      opened_file_ = fopen(file_name, "r");
      fromFileToArrs();
      maxCordinateVal();
      first_file = false;
      fclose(opened_file_);
    } else {
      if (!first_file) reset();
      throw std::invalid_argument("cannot open file");
    }
  } else {
    if (!first_file) reset();
    throw std::invalid_argument("file must have .obj extention");
  }
}

std::size_t s21::SingltonViewer::getCountOfVertexes() noexcept {
  return count_of_vertexes_;
}

std::size_t s21::SingltonViewer::getCountOfIndexes() noexcept {
  return count_of_indexes_;
}

std::size_t s21::SingltonViewer::getCountOfFaces() noexcept {
  return count_of_faces_;
}

const float *s21::SingltonViewer::getVertex() const noexcept {
  return xyz_arr_;
}

const unsigned int *s21::SingltonViewer::getIndex() const noexcept {
  return index_;
}

float s21::SingltonViewer::getMaxCordinateVal() noexcept { return abs_max_; }

void s21::SingltonViewer::scale(float s) noexcept {
  for (std::size_t i = 0; i < count_of_vertexes_ * 3; i++) xyz_arr_[i] *= s;
}

void s21::SingltonViewer::moveXyz(float val, int cordinate) noexcept {
  for (std::size_t i = 0; i < count_of_vertexes_ * 3; i += 3)
    xyz_arr_[i + cordinate] += val;
}

void s21::SingltonViewer::rotateX(float val) noexcept {
  double tY = 0, tZ = 0;
  for (std::size_t i = 0; i < count_of_vertexes_ * 3; i += 3) {
    tY = xyz_arr_[i + Y];
    tZ = xyz_arr_[i + Z];

    xyz_arr_[i + Y] = cos(val) * tY - sin(val) * tZ;
    ;
    xyz_arr_[i + Z] = sin(val) * tY + cos(val) * tZ;
  }
}

void s21::SingltonViewer::rotateY(float val) noexcept {
  double tX = 0, tZ = 0;
  for (std::size_t i = 0; i < count_of_vertexes_ * 3; i += 3) {
    tX = xyz_arr_[i + X];
    tZ = xyz_arr_[i + Z];

    xyz_arr_[i + X] = tX * cos(val) + tZ * sin(val);
    xyz_arr_[i + Z] = -tX * sin(val) + tZ * cos(val);
  }
}

void s21::SingltonViewer::rotateZ(float val) noexcept {
  double tY = 0, tX = 0;
  for (std::size_t i = 0; i < count_of_vertexes_ * 3; i += 3) {
    tX = xyz_arr_[i + X];
    tY = xyz_arr_[i + Y];

    xyz_arr_[i + X] = tX * cos(val) - tY * sin(val);
    xyz_arr_[i + Y] = tX * sin(val) + tY * cos(val);
  }
}

s21::Memento *s21::SingltonViewer::safeState() {
  return new s21::Memento(xyz_arr_, count_of_vertexes_ * 3);
}

void s21::SingltonViewer::undo(s21::Memento *state) {
  if (state && state->array_) {
    for (size_t i = 0; i < count_of_vertexes_ * 3; i++) {
      xyz_arr_[i] = state->array_[i];
    }
  }
}

void s21::SingltonViewer::countFacesVertexes(FILE *file) {
  char str[1024] = {'\0'};
  for (; fgets(str, 1022, file) != NULL;) {
    if (str[0] == 'v' && str[1] == ' ')
      count_of_vertexes_++;
    else if (str[0] == 'f' && str[1] == ' ') {
      count_of_faces_++;
      countIndexesInFace(str + 2);
    }
  }
  fclose(file);
}

void s21::SingltonViewer::countIndexesInFace(char *str) {
  int r = 0;
  for (int i = 0; str[i]; i++, r++)
    while (str[i] != '\n' && str[i] != ' ') i++;
  count_of_indexes_ += (r + r);
}

void s21::SingltonViewer::allocMem() {
  xyz_arr_ = new float[count_of_vertexes_ * 3]{};
  index_ = new unsigned int[count_of_indexes_]{};
}

void s21::SingltonViewer::freeMem() {
  if (xyz_arr_ != nullptr) delete[] xyz_arr_;
  if (index_ != nullptr) delete[] index_;
}

void s21::SingltonViewer::resetNumeric() {
  count_of_faces_ = 0;
  count_of_indexes_ = 0;
  count_of_vertexes_ = 0;
  abs_max_ = 0;
}

void s21::SingltonViewer::reset() {
  first_file = true;
  resetNumeric();
  freeMem();
}

void s21::SingltonViewer::maxCordinateVal() noexcept {
  abs_max_ = fabs(xyz_arr_[X]);

  for (unsigned int i = 0; i < count_of_vertexes_ * 3; i++) {
    if (fabs(xyz_arr_[i]) > abs_max_) abs_max_ = fabs(xyz_arr_[i]);
  }
}

void s21::SingltonViewer::fromFileToArrs() {
  char str[1024] = {'\0'};
  for (int indV = 0, indI = 0; fgets(str, 1022, opened_file_) != NULL;) {
    if (str[0] == 'v' && str[1] == ' ') {
      char *s = str + 2;
      for (int i = 0; i < 3; i++) xyz_arr_[indV++] = std::strtod(s, &s);
    } else if (str[0] == 'f' && str[1] == ' ') {
      indexesToArr(str + 2, &indI);
      indI++;
    }
  }
}

void s21::SingltonViewer::indexesToArr(char *str, int *ind) {
  int st = *ind;
  for (; *str; str++, *ind += 1) {
    index_[*ind] = strtod(str, &str) - 1;
    if (*ind > st) {
      *ind = *ind + 1;
      index_[*ind] = index_[*ind - 1];
    }
    while (*str != '\n' && *str != ' ') str++;
  }
  index_[*ind++] = index_[st];
}