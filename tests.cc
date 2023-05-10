#include <gtest/gtest.h>

#include "./code/Controller.h"

s21::AdapterModel model;
s21::Controller controller(&model);

float tmp_vertex_arr[24] = {1,  -1,       1, -1, -1,        1, -1, -1,
                            -1, -1,       1, -1, -1,        1, 1,  0.999999,
                            1,  1.000001, 1, 1,  -0.999999, 1, -1, -1};

TEST(Viewre, throws) {
  EXPECT_ANY_THROW(controller.readFile("./obj_files/cube.obj"));
  EXPECT_ANY_THROW(controller.readFile("./Makefile"));
  EXPECT_ANY_THROW(controller.readFile("./obj_files/fake.obj"));

  EXPECT_NO_THROW(controller.readFile("./obj_files/hand.OBJ"));
  EXPECT_NO_THROW(controller.readFile("./obj_files/cub.obj"));
}

TEST(Viewre, getters) {
  unsigned int tmp_index_arr[72] = {
      0, 1, 1, 2, 2, 0, 3, 4, 4, 5, 5, 3, 6, 5, 5, 0, 0, 6, 5, 4, 4, 1, 1, 5,
      1, 4, 4, 3, 3, 1, 7, 2, 2, 3, 3, 7, 7, 0, 0, 2, 2, 7, 6, 3, 3, 5, 5, 6,
      7, 6, 6, 0, 0, 7, 0, 5, 5, 1, 1, 0, 2, 1, 1, 3, 3, 2, 6, 7, 7, 3, 3, 6};

  EXPECT_EQ(controller.getCountOfCordinates(), 24);
  EXPECT_EQ(controller.getCountOfFaces(), 12);
  EXPECT_EQ(controller.getCountOfIndexes(), 72);
  ASSERT_FLOAT_EQ(controller.getMaxAbsCordinateValue(), 4.5);

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i], tmp_vertex_arr[i]);
  }

  for (size_t i = 0; i < controller.getCountOfIndexes(); i++) {
    EXPECT_EQ(controller.getIndexArray()[i], tmp_index_arr[i]);
  }
}

TEST(Viewre, modification) {
  controller.moveByXyz(2.45, X);
  controller.moveByXyz(2.45, Y);
  controller.moveByXyz(2.45, Z);

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i], tmp_vertex_arr[i] + 2.45);
  }

  controller.resetModifications();

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i], tmp_vertex_arr[i]);
  }

  controller.setScale(45.8634);

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i],
                    tmp_vertex_arr[i] * 45.8634);
  }

  controller.resetModifications();

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i], tmp_vertex_arr[i]);
  }

  float val = -68.3589;
  val = val * M_PI / 180;

  controller.rotateByX(-68.3589);

  for (size_t i = 0; i < controller.getCountOfCordinates(); i += 3) {
    ASSERT_FLOAT_EQ(
        controller.getVertexArray()[i + Y],
        cos(val) * tmp_vertex_arr[i + Y] - sin(val) * tmp_vertex_arr[i + Z]);
    ASSERT_FLOAT_EQ(
        controller.getVertexArray()[i + Z],
        sin(val) * tmp_vertex_arr[i + Y] + cos(val) * tmp_vertex_arr[i + Z]);
  }

  controller.resetModifications();

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i], tmp_vertex_arr[i]);
  }

  controller.rotateByY(-68.3589);

  for (size_t i = 0; i < controller.getCountOfCordinates(); i += 3) {
    ASSERT_FLOAT_EQ(
        controller.getVertexArray()[i + X],
        tmp_vertex_arr[i + X] * cos(val) + tmp_vertex_arr[i + Z] * sin(val));
    ASSERT_FLOAT_EQ(
        controller.getVertexArray()[i + Z],
        -tmp_vertex_arr[i + X] * sin(val) + tmp_vertex_arr[i + Z] * cos(val));
  }

  controller.resetModifications();

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i], tmp_vertex_arr[i]);
  }

  controller.rotateByZ(-68.3589);

  for (size_t i = 0; i < controller.getCountOfCordinates(); i += 3) {
    ASSERT_FLOAT_EQ(
        controller.getVertexArray()[i + X],
        tmp_vertex_arr[i + X] * cos(val) - tmp_vertex_arr[i + Y] * sin(val));
    ASSERT_FLOAT_EQ(
        controller.getVertexArray()[i + Y],
        tmp_vertex_arr[i + X] * sin(val) + tmp_vertex_arr[i + Y] * cos(val));
  }

  s21::AdapterModel singl;
  s21::Controller test(&singl);

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i], test.getVertexArray()[i]);
  }

  controller.resetModifications();

  for (size_t i = 0; i < controller.getCountOfCordinates(); i++) {
    ASSERT_FLOAT_EQ(controller.getVertexArray()[i], test.getVertexArray()[i]);
  }
}

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
