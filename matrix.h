/******************************************************************
*
* matrix.h
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


#ifndef __MATRIX_H__
#define __MATRIX_H__

void set_identity_matrix(float* result);
void set_rotation_x(float anglex, float* result);
void set_rotation_y(float angley, float* result);
void set_rotation_z(float anglez, float* result);
void set_translation(float x, float y, float z, float* result);
void multiply_matrix(float* m1, float* m2, float* result);
void set_perspective_matrix(float fov, float aspect, float nearPlane, float farPlane, float* result);

#endif // __MATRIX_H__
