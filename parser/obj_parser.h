#ifndef __OBJ_PARSER_H__
#define __OBJ_PARSER_H__

#if __linux
#include <linux/limits.h>
#else
#include <limits.h>
#endif

#include "helper/shared_headers.h"
#include "helper/vertex.h"
#include "list.h"

#define OBJ_LINE_MAX 1024
#define MAX_VERTEX_COUNT 4 //can only handle quads or triangles

#define FREE_ELEMENT(element, count)  \
  do {                                \
    for (int i = 0; i < count; i++) { \
      free(element[i]);               \
    }                                 \
    free(element);                    \
  } while(0);

typedef struct {
  int vertex_index[MAX_VERTEX_COUNT];
  int normal_index[MAX_VERTEX_COUNT];
  int texture_index[MAX_VERTEX_COUNT];
  int vertex_count;
  int material_index;
} obj_face;

typedef struct {
  int pos_index;
  int up_normal_index;
  int equator_normal_index;
  int texture_index[MAX_VERTEX_COUNT];
  int material_index;
} obj_sphere;

typedef struct {
  int pos_index;
  int normal_index;
  int rotation_normal_index;
  int texture_index[MAX_VERTEX_COUNT];
  int material_index;
} obj_plane;

typedef struct {
  double e[3];
} obj_vector;

typedef struct {
  double e[2];
} obj_vector2;

typedef struct {
  char name[NAME_MAX];
  char amb_texture_filename[PATH_MAX];
  char diff_texture_filename[PATH_MAX];
  double amb[3];
  double diff[3];
  double spec[3];
  double reflect;
  double refract;
  double trans;
  double shiny;
  double glossy;
  double refract_index;
} obj_material;

typedef struct {
  int camera_pos_index;
  int camera_look_point_index;
  int camera_up_norm_index;
} obj_camera;

typedef struct {
  int pos_index;
  int material_index;
} obj_light_point;

typedef struct {
  int pos_index;
  int normal_index;
  int material_index;
} obj_light_disc;

typedef struct {
  int vertex_index[MAX_VERTEX_COUNT];
  int material_index;
} obj_light_quad;

typedef struct {
  //  vector extreme_dimensions[2];
  char scene_filename[PATH_MAX];
  char material_filename[PATH_MAX];

  list vertex_list;
  list vertex_normal_list;
  list vertex_texture_list;

  list face_list;
  list sphere_list;
  list plane_list;

  list light_point_list;
  list light_quad_list;
  list light_disc_list;

  list material_list;

  obj_camera *camera;
} obj_growable_scene_data;

typedef struct {
  obj_vector **vertex_list;
  obj_vector **vertex_normal_list;
  obj_vector2 **vertex_texture_list;

  obj_face **face_list;
  obj_sphere **sphere_list;
  obj_plane **plane_list;

  obj_light_point **light_point_list;
  obj_light_quad **light_quad_list;
  obj_light_disc **light_disc_list;

  obj_material **material_list;

//--
  struct object_data obj_data;

  /* Array for holding vertex data of the two models */
  GLfloat *vertex_buffer_data;

  /* Array for holding indices of the two models */
  GLushort *index_buffer_data;
//--

  int vertex_count;
  int vertex_normal_count;
  int vertex_texture_count;

  int face_count;
  int sphere_count;
  int plane_count;

  int light_point_count;
  int light_quad_count;
  int light_disc_count;

  int material_count;

  obj_camera *camera;
} obj_scene_data;

int parse_obj_scene(obj_scene_data *data_out, const char *filename);
void delete_obj_data(obj_scene_data *data_out);

#endif // __OBJ_PARSER_H__
