#include "helper.h"

/******************************************************************
*
* matrix.c
*
* Description: Helper routine for matrix computations.
*
*
* Computer Graphics Proseminar SS 2017
*
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/******************************************************************
*
* set_identity_matrix
*
*******************************************************************/

void set_identity_matrix(float* result) {
  float identity[16] = {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0,
  };

  memcpy(result, identity, 16 * sizeof(float));
}


/******************************************************************
*
* set_rotation_x
*
*******************************************************************/

void set_rotation_x(float anglex, float* result) {
  anglex = deg_to_rad(anglex);

  float temp[16] = {
    1.0,          0.0,           0.0, 0.0,
    0.0, cosf(anglex), -sinf(anglex), 0.0,
    0.0, sinf(anglex),  cosf(anglex), 0.0,
    0.0,          0.0,           0.0, 1.0
  };

  memcpy(result, temp, 16 * sizeof(float));
}


/******************************************************************
*
* set_rotation_y
*
*******************************************************************/

void set_rotation_y(float angley, float* result) {
  angley = deg_to_rad(angley);

  float temp[16] = {
     cosf(angley), 0.0, sinf(angley), 0.0,
              0.0, 1.0,          0.0, 0.0,
    -sinf(angley), 0.0, cosf(angley), 0.0,
              0.0, 0.0,          0.0, 1.0
  };

  memcpy(result, temp, 16 * sizeof(float));
}


/******************************************************************
*
* set_rotation_z
*
*******************************************************************/

void set_rotation_z(float anglez, float* result) {
  anglez = deg_to_rad(anglez);

  float temp[16] = {
    cosf(anglez), -sinf(anglez), 0.0, 0.0,
    sinf(anglez),  cosf(anglez), 0.0, 0.0,
             0.0,           0.0, 1.0, 0.0,
             0.0,           0.0, 0.0, 1.0
  };

  memcpy(result, temp, 16 * sizeof(float));
}


/******************************************************************
*
* set_translation
*
*******************************************************************/

void set_translation(float x, float y, float z, float* result) {
  float temp[16] = {
    1.0, 0.0, 0.0,   x,
    0.0, 1.0, 0.0,   y,
    0.0, 0.0, 1.0,   z,
    0.0, 0.0, 0.0, 1.0
  };

  memcpy(result, temp, 16 * sizeof(float));
}


/******************************************************************
*
* multiply_matrix
*
*******************************************************************/

void multiply_matrix(float* m1, float* m2, float* result) {
  int i;
  float temp[16];

  for (i = 0; i < 16; i++) {
    temp[i] = 0.0;
  }

  temp[0] = m1[0] * m2[0] + m1[1] * m2[4] + m1[2] * m2[8] + m1[3] * m2[12];
  temp[1] = m1[0] * m2[1] + m1[1] * m2[5] + m1[2] * m2[9] + m1[3] * m2[13];
  temp[2] = m1[0] * m2[2] + m1[1] * m2[6] + m1[2] * m2[10] + m1[3] * m2[14];
  temp[3] = m1[0] * m2[3] + m1[1] * m2[7] + m1[2] * m2[11] + m1[3] * m2[15];

  temp[4] = m1[4] * m2[0] + m1[5] * m2[4] + m1[6] * m2[8] + m1[7] * m2[12];
  temp[5] = m1[4] * m2[1] + m1[5] * m2[5] + m1[6] * m2[9] + m1[7] * m2[13];
  temp[6] = m1[4] * m2[2] + m1[5] * m2[6] + m1[6] * m2[10] + m1[7] * m2[14];
  temp[7] = m1[4] * m2[3] + m1[5] * m2[7] + m1[6] * m2[11] + m1[7] * m2[15];

  temp[8] = m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[8] + m1[11] * m2[12];
  temp[9] = m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[9] + m1[11] * m2[13];
  temp[10] = m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14];
  temp[11] = m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15];

  temp[12] = m1[12] * m2[0] + m1[13] * m2[4] + m1[14] * m2[8] + m1[15] * m2[12];
  temp[13] = m1[12] * m2[1] + m1[13] * m2[5] + m1[14] * m2[9] + m1[15] * m2[13];
  temp[14] = m1[12] * m2[2] + m1[13] * m2[6] + m1[14] * m2[10] + m1[15] * m2[14];
  temp[15] = m1[12] * m2[3] + m1[13] * m2[7] + m1[14] * m2[11] + m1[15] * m2[15];

  memcpy(result, temp, 16 * sizeof(float));
}


/******************************************************************
*
* set_perspective_matrix
*
*******************************************************************/

void set_perspective_matrix(float fov, float aspect, float nearPlane, float farPlane, float* result) {
  float f = 1.0 / tan(fov * M_PI / 360.0);
  float c1 = -(farPlane + nearPlane) / (farPlane - nearPlane);
  float c2 = -(2.0 * farPlane * nearPlane) / (farPlane - nearPlane);

  float temp[16] = {
    f/aspect, 0.0,  0.0, 0.0,
         0.0,   f,  0.0, 0.0,
         0.0, 0.0,   c1,  c2,
         0.0, 0.0, -1.0, 0.0
  };

  memcpy(result, temp, 16 * sizeof(float));
}
