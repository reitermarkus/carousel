#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "opengl.h"
#include "helper.h"
#include "vertex.h"

void cube(double h, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {

  *vertices_size = 8 * sizeof(struct vertex);
  *vertices = malloc(*vertices_size);

  *indices_size = 12 * 3 * sizeof(GLushort);
  *indices = malloc(*indices_size);

  SET_VERTEX_POSITION((*vertices)[0], -h, -h, h);
  SET_VERTEX_COLOR((*vertices)[0], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[1], h, -h, h);
  SET_VERTEX_COLOR((*vertices)[1], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[2], h, h, h);
  SET_VERTEX_COLOR((*vertices)[2], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[3], -h, h, h);
  SET_VERTEX_COLOR((*vertices)[3], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[4], -h, -h, -h);
  SET_VERTEX_COLOR((*vertices)[4], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[5], h, -h, -h);
  SET_VERTEX_COLOR((*vertices)[5], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[6], h, h, -h);
  SET_VERTEX_COLOR((*vertices)[6], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[7], -h, h, -h);
  SET_VERTEX_COLOR((*vertices)[7], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));

  (*indices)[0] = 0; (*indices)[1] = 1; (*indices)[2] = 2; (*indices)[3] = 2; (*indices)[4] = 3; (*indices)[5] = 0;
  (*indices)[6] = 1; (*indices)[7] = 5; (*indices)[8] = 6; (*indices)[9] = 6; (*indices)[10] = 2; (*indices)[11] = 1;
  (*indices)[12] = 7; (*indices)[13] = 6; (*indices)[14] = 5; (*indices)[15] = 5; (*indices)[16] = 4; (*indices)[17] = 7;
  (*indices)[18] = 4; (*indices)[19] = 0; (*indices)[20] = 3; (*indices)[21] = 3; (*indices)[22] = 7; (*indices)[23] = 4;
  (*indices)[24] = 4; (*indices)[25] = 5; (*indices)[26] = 1; (*indices)[27] = 1; (*indices)[28] = 0; (*indices)[29] = 4;
  (*indices)[30] = 3; (*indices)[31] = 2; (*indices)[32] = 6; (*indices)[33] = 6; (*indices)[34] = 7; (*indices)[35] = 3;
}