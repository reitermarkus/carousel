#include "cuboid.h"

#include "helper/macros.h"

void cuboid(float w, float h, float d, struct object_data* obj) {
  obj->vertex_count = 8;
  obj->vertices = malloc(obj->vertex_count * sizeof(*obj->vertices));

  obj->index_count = 12;
  obj->indices = malloc(obj->index_count * sizeof(*obj->indices));

  SET_VERTEX_POSITION(obj->vertices[0], -w, -h, d);
  SET_VERTEX_COLOR(obj->vertices[0], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  SET_VERTEX_POSITION(obj->vertices[1], w, -h, d);
  SET_VERTEX_COLOR(obj->vertices[1], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  SET_VERTEX_POSITION(obj->vertices[2], w, h, d);
  SET_VERTEX_COLOR(obj->vertices[2], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  SET_VERTEX_POSITION(obj->vertices[3], -w, h, d);
  SET_VERTEX_COLOR(obj->vertices[3], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  SET_VERTEX_POSITION(obj->vertices[4], -w, -h, -d);
  SET_VERTEX_COLOR(obj->vertices[4], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  SET_VERTEX_POSITION(obj->vertices[5], w, -h, -d);
  SET_VERTEX_COLOR(obj->vertices[5], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  SET_VERTEX_POSITION(obj->vertices[6], w, h, -d);
  SET_VERTEX_COLOR(obj->vertices[6], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  SET_VERTEX_POSITION(obj->vertices[7], -w, h, -d);
  SET_VERTEX_COLOR(obj->vertices[7], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);

  // Front side.
  obj->indices[0].a = 0;
  obj->indices[0].b = 1;
  obj->indices[0].c = 2;
  obj->indices[1].a = 2;
  obj->indices[1].b = 3;
  obj->indices[1].c = 0;

  // Right side.
  obj->indices[2].a = 1;
  obj->indices[2].b = 5;
  obj->indices[2].c = 6;
  obj->indices[3].a = 6;
  obj->indices[3].b = 2;
  obj->indices[3].c = 1;

  // Backside.
  obj->indices[4].a = 7;
  obj->indices[4].b = 6;
  obj->indices[4].c = 5;
  obj->indices[5].a = 5;
  obj->indices[5].b = 4;
  obj->indices[5].c = 7;

  // Left side.
  obj->indices[6].a = 4;
  obj->indices[6].b = 0;
  obj->indices[6].c = 3;
  obj->indices[7].a = 3;
  obj->indices[7].b = 7;
  obj->indices[7].c = 4;

  // Bottom side.
  obj->indices[8].a = 4;
  obj->indices[8].b = 5;
  obj->indices[8].c = 1;
  obj->indices[9].a = 1;
  obj->indices[9].b = 0;
  obj->indices[9].c = 4;

  // Top side.
  obj->indices[10].a = 3;
  obj->indices[10].b = 2;
  obj->indices[10].c = 6;
  obj->indices[11].a = 6;
  obj->indices[11].b = 7;
  obj->indices[11].c = 3;
}
