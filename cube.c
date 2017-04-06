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

  (*vertices)[0].position.x = -h;
  (*vertices)[0].position.y = -h;
  (*vertices)[0].position.z = h;
  (*vertices)[0].color.r = R(RGB_RAND);
  (*vertices)[0].color.g = G(RGB_RAND);
  (*vertices)[0].color.b = B(RGB_RAND);
  (*vertices)[1].position.x = h;
  (*vertices)[1].position.y = -h;
  (*vertices)[1].position.z = h;
  (*vertices)[1].color.r = R(RGB_RAND);
  (*vertices)[1].color.g = G(RGB_RAND);
  (*vertices)[1].color.b = B(RGB_RAND);
  (*vertices)[2].position.x = h;
  (*vertices)[2].position.y = h;
  (*vertices)[2].position.z = h;
  (*vertices)[2].color.r = R(RGB_RAND);
  (*vertices)[2].color.g = G(RGB_RAND);
  (*vertices)[2].color.b = B(RGB_RAND);
  (*vertices)[3].position.x = -h;
  (*vertices)[3].position.y = h;
  (*vertices)[3].position.z = h;
  (*vertices)[3].color.r = R(RGB_RAND);
  (*vertices)[3].color.g = G(RGB_RAND);
  (*vertices)[3].color.b = B(RGB_RAND);
  (*vertices)[4].position.x = -h;
  (*vertices)[4].position.y = -h;
  (*vertices)[4].position.z = -h;
  (*vertices)[4].color.r = R(RGB_RAND);
  (*vertices)[4].color.g = G(RGB_RAND);
  (*vertices)[4].color.b = B(RGB_RAND);
  (*vertices)[5].position.x = h;
  (*vertices)[5].position.y = -h;
  (*vertices)[5].position.z = -h;
  (*vertices)[5].color.r = R(RGB_RAND);
  (*vertices)[5].color.g = G(RGB_RAND);
  (*vertices)[5].color.b = B(RGB_RAND);
  (*vertices)[6].position.x = h;
  (*vertices)[6].position.y = h;
  (*vertices)[6].position.z = -h;
  (*vertices)[6].color.r = R(RGB_RAND);
  (*vertices)[6].color.g = G(RGB_RAND);
  (*vertices)[6].color.b = B(RGB_RAND);
  (*vertices)[7].position.x = -h;
  (*vertices)[7].position.y = h;
  (*vertices)[7].position.z = -h;
  (*vertices)[7].color.r = R(RGB_RAND);
  (*vertices)[7].color.g = G(RGB_RAND);
  (*vertices)[7].color.b = B(RGB_RAND);

  (*indices)[0] = 0; (*indices)[1] = 1; (*indices)[2] = 2; (*indices)[3] = 2; (*indices)[4] = 3; (*indices)[5] = 0;
  (*indices)[6] = 1; (*indices)[7] = 5; (*indices)[8] = 6; (*indices)[9] = 6; (*indices)[10] = 2; (*indices)[11] = 1;
  (*indices)[12] = 7; (*indices)[13] = 6; (*indices)[14] = 5; (*indices)[15] = 5; (*indices)[16] = 4; (*indices)[17] = 7;
  (*indices)[18] = 4; (*indices)[19] = 0; (*indices)[20] = 3; (*indices)[21] = 3; (*indices)[22] = 7; (*indices)[23] = 4;
  (*indices)[24] = 4; (*indices)[25] = 5; (*indices)[26] = 1; (*indices)[27] = 1; (*indices)[28] = 0; (*indices)[29] = 4;
  (*indices)[30] = 3; (*indices)[31] = 2; (*indices)[32] = 6; (*indices)[33] = 6; (*indices)[34] = 7; (*indices)[35] = 3;
}
