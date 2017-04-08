#ifndef __MATRIX_H__
#define __MATRIX_H__

void matrix_print(float* matrix);
void matrix_identity(float* matrix);
void matrix_multiply(float* m1, float* m2, float* matrix);

void matrix_rotation_x(float angle, float* matrix);
void matrix_rotation_y(float angle, float* matrix);
void matrix_rotation_z(float angle, float* matrix);
void matrix_rotate_x(float angle, float* matrix);
void matrix_rotate_y(float angle, float* matrix);
void matrix_rotate_z(float angle, float* matrix);

void matrix_translation(float x, float y, float z, float* matrix);
void matrix_translate_x(float x, float* matrix);
void matrix_translate_y(float y, float* matrix);
void matrix_translate_z(float z, float* matrix);

void matrix_perspective(float fov, float aspect, float near_plane, float far_plane, float* matrix);

#endif // __MATRIX_H__
