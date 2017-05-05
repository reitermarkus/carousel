#include "vertex.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static struct normal face_normal(struct position p1, struct position p2, struct position p3) {
  struct position u = {
    p2.x - p1.x,
    p2.y - p1.y,
    p2.z - p1.z
  };

  struct position v = {
    p3.x - p1.x,
    p3.y - p1.y,
    p3.z - p1.z
  };

  return (struct normal){
  	(u.y * v.z) - (u.z * v.y),
  	(u.z * v.x) - (u.x * v.z),
  	(u.x * v.y) - (u.y * v.x),
  };
}

static void calculate_face_normals(struct object_data* obj) {
  puts("Calculating face normals ...");

  obj->face_normals = malloc(obj->index_count * sizeof(*obj->face_normals));

  for (int i = 0; i < obj->index_count; i++) {
    obj->face_normals[i] = face_normal(
      obj->vertices[obj->indices[i].a].position,
      obj->vertices[obj->indices[i].b].position,
      obj->vertices[obj->indices[i].c].position
    );

    printf("%f\t%f\t%f\n", obj->face_normals[i].a, obj->face_normals[i].b, obj->face_normals[i].c);
  }
}

static struct normal* normalize(struct normal* normal) {
  float factor = sqrt(pow(normal->a, 2) + pow(normal->b, 2) + pow(normal->c, 2));

  normal->a /= factor;
  normal->b /= factor;
  normal->c /= factor;

  return normal;
}

static void calculate_vertex_normals(struct object_data* obj) {
  puts("Calculating vertex normals ...");

  for(int i = 0; i < obj->vertex_count; i++) {
    struct normal normal_sum = {0, 0, 0};

    for(int j = 0; j < obj->index_count; j++) {
      if ((obj->indices[j].a == i) || (obj->indices[j].b == i) || (obj->indices[j].c == i)) {
        normal_sum.a += obj->face_normals[j].a;
        normal_sum.b += obj->face_normals[j].b;
        normal_sum.c += obj->face_normals[j].c;
      }
    }

    obj->vertices[i].normal = *normalize(&normal_sum);

    printf("%f\t%f\t%f\n", obj->vertices[i].normal.a, obj->vertices[i].normal.b, obj->vertices[i].normal.c);
  }
}

void calculate_normals(struct object_data* obj) {
  calculate_face_normals(obj);
  calculate_vertex_normals(obj);
}
