#include "rectangle.h"

#include "helper/macros.h"

void rectangle(float w, float h, struct object_data* obj) {
  w /= 2.0;
  h /= 2.0;

  obj->vertex_count = 4;
  obj->vertices = malloc(obj->vertex_count * sizeof(*obj->vertices));

  obj->index_count = 4;
  obj->indices = malloc(obj->index_count * sizeof(*obj->indices));

  SET_VERTEX_POSITION(obj->vertices[0], -1 * w, -1 * h,  0);
  SET_VERTEX_POSITION(obj->vertices[1],  1 * w, -1 * h,  0);
  SET_VERTEX_POSITION(obj->vertices[2],  1 * w,  1 * h,  0);
  SET_VERTEX_POSITION(obj->vertices[3], -1 * w,  1 * h,  0);

  obj->indices[0].a = obj->indices[2].c = 0;
  obj->indices[0].b = obj->indices[2].b = 1;
  obj->indices[0].c = obj->indices[2].a = 2;

  obj->indices[1].a = obj->indices[3].c = 0;
  obj->indices[1].b = obj->indices[3].b = 2;
  obj->indices[1].c = obj->indices[3].a = 3;

  for (size_t i = 0; i < obj->vertex_count; i++) {
    SET_VERTEX_COLOR(obj->vertices[i], 1.0, 1.0, 1.0, 1.0);
  }
}
