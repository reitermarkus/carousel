#include "cuboid.h"

#include "helper/macros.h"

void cuboid(float w, float h, float d, struct object_data* obj) {
  w /= 2.0;
  h /= 2.0;
  d /= 2.0;

  obj->vertex_count = 24; // 6 * 4
  obj->vertices = malloc(obj->vertex_count * sizeof(*obj->vertices));

  obj->index_count = 12;
  obj->indices = malloc(obj->index_count * sizeof(*obj->indices));

  // Front side.
  SET_VERTEX_POSITION(obj->vertices[0],  -1 * w, -1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[1],   1 * w, -1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[2],   1 * w,  1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[3],  -1 * w,  1 * h,  1 * d);

  // Backside.
  SET_VERTEX_POSITION(obj->vertices[4],   1 * w, -1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[5],  -1 * w, -1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[6],  -1 * w,  1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[7],   1 * w,  1 * h, -1 * d);

  // Bottom side.
  SET_VERTEX_POSITION(obj->vertices[8],  -1 * w, -1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[9],   1 * w, -1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[10],  1 * w, -1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[11], -1 * w, -1 * h,  1 * d);

  // Top side.
  SET_VERTEX_POSITION(obj->vertices[12], -1 * w,  1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[13],  1 * w,  1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[14],  1 * w,  1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[15], -1 * w,  1 * h, -1 * d);

  // Right side.
  SET_VERTEX_POSITION(obj->vertices[16],  1 * w, -1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[17],  1 * w, -1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[18],  1 * w,  1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[19],  1 * w,  1 * h,  1 * d);

  // Left side.
  SET_VERTEX_POSITION(obj->vertices[20], -1 * w, -1 * h, -1 * d);
  SET_VERTEX_POSITION(obj->vertices[21], -1 * w, -1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[22], -1 * w,  1 * h,  1 * d);
  SET_VERTEX_POSITION(obj->vertices[23], -1 * w,  1 * h, -1 * d);

  for (size_t i = 0; i < 6; i++) {
    for (size_t j = 0; j < 2; j++) {
      obj->indices[i * 2 + j].a = i * 4 + 0;
      obj->indices[i * 2 + j].b = i * 4 + 1 + j;
      obj->indices[i * 2 + j].c = i * 4 + 2 + j;
    }
  }

  for (size_t i = 0; i < 24; i++) {
    SET_VERTEX_COLOR(obj->vertices[i], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  }
}
