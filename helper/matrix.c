#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "matrix.h"

void matrix_print(float* matrix) {
  printf("┌──────────┬──────────┬──────────┬──────────┐\n");

  for (int i = 0; i < 16; i += 4) {
    if (i > 0) {
      printf("├──────────┼──────────┼──────────┼──────────┤\n");
    }

    printf("| %8f | %8f | %8f | %8f |\n", matrix[i], matrix[i + 1], matrix[i + 2], matrix[i + 3]);
  }

  printf("└──────────┴──────────┴──────────┴──────────┘\n");
}

void matrix_identity(float* matrix) {
  float identity[16] = {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0,
  };

  memcpy(matrix, identity, 16 * sizeof(float));
}

void matrix_rotation_x(float angle, float* matrix) {
  float temp[16] = {
    1.0,        0.0,         0.0, 0.0,
    0.0, cos(angle), -sin(angle), 0.0,
    0.0, sin(angle),  cos(angle), 0.0,
    0.0,        0.0,         0.0, 1.0,
  };

  memcpy(matrix, temp, 16 * sizeof(float));
}

void matrix_rotate_x(float angle, float* matrix) {
  float rotation[16];
  matrix_rotation_x(angle, rotation);
  matrix_multiply(rotation, matrix, matrix);
}

void matrix_rotation_y(float angle, float* matrix) {
  float temp[16] = {
     cos(angle), 0.0, sin(angle), 0.0,
            0.0, 1.0,        0.0, 0.0,
    -sin(angle), 0.0, cos(angle), 0.0,
            0.0, 0.0,        0.0, 1.0,
  };

  memcpy(matrix, temp, 16 * sizeof(float));
}

void matrix_rotate_y(float angle, float* matrix) {
  float rotation[16];
  matrix_rotation_y(angle, rotation);
  matrix_multiply(rotation, matrix, matrix);
}

void matrix_rotation_z(float angle, float* matrix) {
  float temp[16] = {
    cos(angle), -sin(angle), 0.0, 0.0,
    sin(angle),  cos(angle), 0.0, 0.0,
           0.0,         0.0, 1.0, 0.0,
           0.0,         0.0, 0.0, 1.0,
  };

  memcpy(matrix, temp, 16 * sizeof(float));
}

void matrix_rotate_z(float angle, float* matrix) {
  float rotation[16];
  matrix_rotation_z(angle, rotation);
  matrix_multiply(rotation, matrix, matrix);
}

void matrix_translation(float x, float y, float z, float* matrix) {
  float temp[16] = {
    1.0, 0.0, 0.0,   x,
    0.0, 1.0, 0.0,   y,
    0.0, 0.0, 1.0,   z,
    0.0, 0.0, 0.0, 1.0,
  };

  memcpy(matrix, temp, 16 * sizeof(float));
}

void matrix_translate_x(float x, float* matrix) {
  float temp[16];
  matrix_translation(x, 0, 0, temp);
  matrix_multiply(temp, matrix, matrix);
}

void matrix_translate_y(float y, float* matrix) {
  float temp[16];
  matrix_translation(0, y, 0, temp);
  matrix_multiply(temp, matrix, matrix);
}

void matrix_translate_z(float z, float* matrix) {
  float temp[16];
  matrix_translation(0, 0, z, temp);
  matrix_multiply(temp, matrix, matrix);
}

void matrix_multiply(float* m1, float* m2, float* matrix) {
  float temp[16];

  for (int i = 0; i < 16; i++) {
    temp[i] = 0.0;
  }

  temp[0]  =  m1[0] * m2[0] +  m1[1] * m2[4] +  m1[2] *  m2[8] +  m1[3] * m2[12];
  temp[1]  =  m1[0] * m2[1] +  m1[1] * m2[5] +  m1[2] *  m2[9] +  m1[3] * m2[13];
  temp[2]  =  m1[0] * m2[2] +  m1[1] * m2[6] +  m1[2] * m2[10] +  m1[3] * m2[14];
  temp[3]  =  m1[0] * m2[3] +  m1[1] * m2[7] +  m1[2] * m2[11] +  m1[3] * m2[15];

  temp[4]  =  m1[4] * m2[0] +  m1[5] * m2[4] +  m1[6] *  m2[8] +  m1[7] * m2[12];
  temp[5]  =  m1[4] * m2[1] +  m1[5] * m2[5] +  m1[6] *  m2[9] +  m1[7] * m2[13];
  temp[6]  =  m1[4] * m2[2] +  m1[5] * m2[6] +  m1[6] * m2[10] +  m1[7] * m2[14];
  temp[7]  =  m1[4] * m2[3] +  m1[5] * m2[7] +  m1[6] * m2[11] +  m1[7] * m2[15];

  temp[8]  =  m1[8] * m2[0] +  m1[9] * m2[4] + m1[10] *  m2[8] + m1[11] * m2[12];
  temp[9]  =  m1[8] * m2[1] +  m1[9] * m2[5] + m1[10] *  m2[9] + m1[11] * m2[13];
  temp[10] =  m1[8] * m2[2] +  m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14];
  temp[11] =  m1[8] * m2[3] +  m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15];

  temp[12] = m1[12] * m2[0] + m1[13] * m2[4] + m1[14] *  m2[8] + m1[15] * m2[12];
  temp[13] = m1[12] * m2[1] + m1[13] * m2[5] + m1[14] *  m2[9] + m1[15] * m2[13];
  temp[14] = m1[12] * m2[2] + m1[13] * m2[6] + m1[14] * m2[10] + m1[15] * m2[14];
  temp[15] = m1[12] * m2[3] + m1[13] * m2[7] + m1[14] * m2[11] + m1[15] * m2[15];

  memcpy(matrix, temp, 16 * sizeof(float));
}

void matrix_perspective(float fov, float aspect, float near_plane, float far_plane, float* matrix) {
  float f = 1.0 / tan(fov / 2.0);
  float c1 = -(far_plane + near_plane) / (far_plane - near_plane);
  float c2 = -(2.0 * far_plane * near_plane) / (far_plane - near_plane);

  float temp[16] = {
    f/aspect, 0.0,  0.0, 0.0,
         0.0,   f,  0.0, 0.0,
         0.0, 0.0,   c1,  c2,
         0.0, 0.0, -1.0, 0.0,
  };

  memcpy(matrix, temp, 16 * sizeof(float));
}
