#include "cuboid.h"

#include "helper/macros.h"

void cuboid(float w, float h, float d, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count) {
  *vertex_count = 8;
  *vertices = malloc(*vertex_count * sizeof(**vertices));

  *index_count = 12;
  *indices = malloc(*index_count * sizeof(**indices));

  SET_VERTEX_POSITION((*vertices)[0], -w, -h, d);
  SET_VERTEX_COLOR((*vertices)[0], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[1], w, -h, d);
  SET_VERTEX_COLOR((*vertices)[1], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[2], w, h, d);
  SET_VERTEX_COLOR((*vertices)[2], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[3], -w, h, d);
  SET_VERTEX_COLOR((*vertices)[3], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[4], -w, -h, -d);
  SET_VERTEX_COLOR((*vertices)[4], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[5], w, -h, -d);
  SET_VERTEX_COLOR((*vertices)[5], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[6], w, h, -d);
  SET_VERTEX_COLOR((*vertices)[6], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  SET_VERTEX_POSITION((*vertices)[7], -w, h, -d);
  SET_VERTEX_COLOR((*vertices)[7], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));

  // Front side.
  (*indices)[0].a = 0;
  (*indices)[0].b = 1;
  (*indices)[0].c = 2;
  (*indices)[1].a = 2;
  (*indices)[1].b = 3;
  (*indices)[1].c = 0;

  // Right side.
  (*indices)[2].a = 1;
  (*indices)[2].b = 5;
  (*indices)[2].c = 6;
  (*indices)[3].a = 6;
  (*indices)[3].b = 2;
  (*indices)[3].c = 1;

  // Backside.
  (*indices)[4].a = 7;
  (*indices)[4].b = 6;
  (*indices)[4].c = 5;
  (*indices)[5].a = 5;
  (*indices)[5].b = 4;
  (*indices)[5].c = 7;

  // Left side.
  (*indices)[6].a = 4;
  (*indices)[6].b = 0;
  (*indices)[6].c = 3;
  (*indices)[7].a = 3;
  (*indices)[7].b = 7;
  (*indices)[7].c = 4;

  // Bottom side.
  (*indices)[8].a = 4;
  (*indices)[8].b = 5;
  (*indices)[8].c = 1;
  (*indices)[9].a = 1;
  (*indices)[9].b = 0;
  (*indices)[9].c = 4;

  // Top side.
  (*indices)[10].a = 3;
  (*indices)[10].b = 2;
  (*indices)[10].c = 6;
  (*indices)[11].a = 6;
  (*indices)[11].b = 7;
  (*indices)[11].c = 3;
}
