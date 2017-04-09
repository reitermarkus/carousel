#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef float matrix[4][4];

void matrix_print(const matrix m);
void matrix_identity(matrix result_matrix);
void matrix_multiply(const matrix m1, const matrix m2, matrix result_matrix);

void matrix_rotation_x(float angle, matrix result_matrix);
void matrix_rotation_y(float angle, matrix result_matrix);
void matrix_rotation_z(float angle, matrix result_matrix);
void matrix_rotate_x(float angle, matrix result_matrix);
void matrix_rotate_y(float angle, matrix result_matrix);
void matrix_rotate_z(float angle, matrix result_matrix);

void matrix_translation(float x, float y, float z, matrix result_matrix);
void matrix_translate_x(float x, matrix result_matrix);
void matrix_translate_y(float y, matrix result_matrix);
void matrix_translate_z(float z, matrix result_matrix);

void matrix_perspective(float fov, float aspect, float near_plane, float far_plane, matrix result_matrix);

#endif // __MATRIX_H__
