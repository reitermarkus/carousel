#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "matrix.h"

void matrix_print(const matrix m) {
  printf("┌──────────┬──────────┬──────────┬──────────┐\n");

  for (int i = 0; i < 4; i++) {
    if (i > 0) {
      printf("├──────────┼──────────┼──────────┼──────────┤\n");
    }

    printf("| %8f | %8f | %8f | %8f |\n", m[i][0], m[i][1], m[i][2], m[i][3]);
  }

  printf("└──────────┴──────────┴──────────┴──────────┘\n");
}

void matrix_identity(matrix result_matrix) {
  matrix identity = {
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0},
  };

  memcpy(result_matrix, identity, sizeof(matrix));
}

void matrix_rotation_x(float angle, matrix result_matrix) {
  matrix temp = {
    {1.0, 0.0,        0.0,         0.0},
    {0.0, cos(angle), -sin(angle), 0.0},
    {0.0, sin(angle), cos(angle),  0.0},
    {0.0, 0.0,        0.0,         1.0},
  };

  memcpy(result_matrix, temp, sizeof(matrix));
}

void matrix_rotate_x(float angle, matrix result_matrix) {
  matrix rotation;
  matrix_rotation_x(angle, rotation);
  matrix_multiply(rotation, result_matrix, result_matrix);
}

void matrix_rotation_y(float angle, matrix result_matrix) {
  matrix temp = {
    {cos(angle),  0.0, sin(angle), 0.0},
    {0.0,         1.0, 0.0,        0.0},
    {-sin(angle), 0.0, cos(angle), 0.0},
    {0.0,         0.0, 0.0,        1.0},
  };

  memcpy(result_matrix, temp, sizeof(matrix));
}

void matrix_rotate_y(float angle, matrix result_matrix) {
  matrix rotation;
  matrix_rotation_y(angle, rotation);
  matrix_multiply(rotation, result_matrix, result_matrix);
}

void matrix_rotation_z(float angle, matrix result_matrix) {
  matrix temp = {
    {cos(angle), -sin(angle), 0.0, 0.0},
    {sin(angle), cos(angle),  0.0, 0.0},
    {0.0,        0.0,         1.0, 0.0},
    {0.0,        0.0,         0.0, 1.0},
  };

  memcpy(result_matrix, temp, sizeof(matrix));
}

void matrix_rotate_z(float angle, matrix result_matrix) {
  matrix rotation;
  matrix_rotation_z(angle, rotation);
  matrix_multiply(rotation, result_matrix, result_matrix);
}

void matrix_translation(float x, float y, float z, matrix result_matrix) {
  matrix temp = {
    {1.0, 0.0, 0.0, x  },
    {0.0, 1.0, 0.0, y  },
    {0.0, 0.0, 1.0, z  },
    {0.0, 0.0, 0.0, 1.0},
  };

  memcpy(result_matrix, temp, sizeof(matrix));
}

void matrix_translate_x(float x, matrix result_matrix) {
  matrix temp;
  matrix_translation(x, 0, 0, temp);
  matrix_multiply(temp, result_matrix, result_matrix);
}

void matrix_translate_y(float y, matrix result_matrix) {
  matrix temp;
  matrix_translation(0, y, 0, temp);
  matrix_multiply(temp, result_matrix, result_matrix);
}

void matrix_translate_z(float z, matrix result_matrix) {
  matrix temp;
  matrix_translation(0, 0, z, temp);
  matrix_multiply(temp, result_matrix, result_matrix);
}

void matrix_multiply(const matrix m1, const matrix m2, matrix result_matrix) {
  matrix temp = {
    {
      m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m1[0][3] * m2[3][0],
      m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m1[0][3] * m2[3][1],
      m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2] + m1[0][3] * m2[3][2],
      m1[0][0] * m2[0][3] + m1[0][1] * m2[1][3] + m1[0][2] * m2[2][3] + m1[0][3] * m2[3][3],
    },
    {
      m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0] + m1[1][3] * m2[3][0],
      m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1] + m1[1][3] * m2[3][1],
      m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2] + m1[1][3] * m2[3][2],
      m1[1][0] * m2[0][3] + m1[1][1] * m2[1][3] + m1[1][2] * m2[2][3] + m1[1][3] * m2[3][3],
    },
    {
      m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0] + m1[2][3] * m2[3][0],
      m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1] + m1[2][3] * m2[3][1],
      m1[2][0] * m2[0][2] + m1[2][1] * m2[1][2] + m1[2][2] * m2[2][2] + m1[2][3] * m2[3][2],
      m1[2][0] * m2[0][3] + m1[2][1] * m2[1][3] + m1[2][2] * m2[2][3] + m1[2][3] * m2[3][3],
    },
    {
      m1[3][0] * m2[0][0] + m1[3][1] * m2[1][0] + m1[3][2] * m2[2][0] + m1[3][3] * m2[3][0],
      m1[3][0] * m2[0][1] + m1[3][1] * m2[1][1] + m1[3][2] * m2[2][1] + m1[3][3] * m2[3][1],
      m1[3][0] * m2[0][2] + m1[3][1] * m2[1][2] + m1[3][2] * m2[2][2] + m1[3][3] * m2[3][2],
      m1[3][0] * m2[0][3] + m1[3][1] * m2[1][3] + m1[3][2] * m2[2][3] + m1[3][3] * m2[3][3],
    }
  };

  memcpy(result_matrix, temp, sizeof(matrix));
}

void matrix_perspective(float fov, float aspect, float near_plane, float far_plane, matrix result_matrix) {
  float f = 1.0 / tan(fov / 2.0);
  float c1 = -(far_plane + near_plane) / (far_plane - near_plane);
  float c2 = -(2.0 * far_plane * near_plane) / (far_plane - near_plane);

  float temp[16] = {
    f/aspect, 0.0,  0.0, 0.0,
         0.0,   f,  0.0, 0.0,
         0.0, 0.0,   c1,  c2,
         0.0, 0.0, -1.0, 0.0,
  };

  memcpy(result_matrix, temp, sizeof(matrix));
}
