#ifndef __MATRIX_H__
#define __MATRIX_H__

void print_matrix(float* matrix);
void set_identity_matrix(float* matrix);
void set_rotation_x(float angle, float* matrix);
void set_rotation_y(float angle, float* matrix);
void set_rotation_z(float angle, float* matrix);
void rotate_x(float angle, float* matrix);
void rotate_y(float angle, float* matrix);
void rotate_z(float angle, float* matrix);
void set_translation(float x, float y, float z, float* matrix);
void translate_x(float x, float* matrix);
void translate_y(float y, float* matrix);
void translate_z(float z, float* matrix);
void multiply_matrix(float* m1, float* m2, float* matrix);
void set_perspective_matrix(float fov, float aspect, float nearPlane, float farPlane, float* matrix);

#endif // __MATRIX_H__
