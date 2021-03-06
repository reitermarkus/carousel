#define _XOPEN_SOURCE 600
#include "obj_parser.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alloca.h>
#include <errno.h>
#include <libgen.h>

#include "list.h"

static const char* OBJ_WHITESPACE = " \t\n\r";
static const char* MTL_WHITESPACE = " \t";

void obj_free_half_list(list *listo) {
  list_delete_all(listo);
  free(listo->names);
}

int obj_convert_to_list_index(int current_max, int index) {
  if (index == 0) {
    //no index
    return -1;
  }

  if (index < 0) {
    //relative to current list position
    return current_max + index;
  }

  //normal counting index
  return index - 1;
}

void obj_convert_to_list_index_v(int current_max, int *indices) {
  for (int i=0; i < MAX_VERTEX_COUNT; i++) {
    indices[i] = obj_convert_to_list_index(current_max, indices[i]);
  }
}

void obj_set_material_defaults(obj_material *mtl) {
  mtl->amb[0] = 0.2;
  mtl->amb[1] = 0.2;
  mtl->amb[2] = 0.2;
  mtl->diff[0] = 0.8;
  mtl->diff[1] = 0.8;
  mtl->diff[2] = 0.8;
  mtl->spec[0] = 1.0;
  mtl->spec[1] = 1.0;
  mtl->spec[2] = 1.0;
  mtl->reflect = 0.0;
  mtl->trans = 1;
  mtl->glossy = 98;
  mtl->shiny = 0;
  mtl->refract_index = 1;
  mtl->amb_texture_filename[0] = '\0';
  mtl->diff_texture_filename[0] = '\0';
}

int obj_parse_vertex_index(int *vertex_index, int *texture_index, int *normal_index) {
  char *temp_str;
  char *token;
  int vertex_count = 0;

  while ((token = strtok(NULL, OBJ_WHITESPACE)) != NULL) {
    if (texture_index != NULL) {
      texture_index[vertex_count] = 0;
    }

    if (normal_index != NULL) {
      normal_index[vertex_count] = 0;
    }

    vertex_index[vertex_count] = atoi(token);

    if (strstr(token, "//")) {
       //normal only
      temp_str = strchr(token, '/');
      temp_str++;
      normal_index[vertex_count] = atoi(++temp_str);
    } else if (strstr(token, "/")) {
      temp_str = strchr(token, '/');
      texture_index[vertex_count] = atoi(++temp_str);

      if (strstr(temp_str, "/")) {
        temp_str = strchr(temp_str, '/');
        normal_index[vertex_count] = atoi(++temp_str);
      }
    }

    vertex_count++;
  }

  return vertex_count;
}

obj_face* obj_parse_face(obj_growable_scene_data *scene) {
  int vertex_count;
  obj_face *face = (obj_face*)malloc(sizeof(obj_face));

  vertex_count = obj_parse_vertex_index(face->vertex_index, face->texture_index, face->normal_index);
  obj_convert_to_list_index_v(scene->vertex_list.item_count, face->vertex_index);
  obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, face->texture_index);
  obj_convert_to_list_index_v(scene->vertex_normal_list.item_count, face->normal_index);
  face->vertex_count = vertex_count;

  return face;
}

obj_sphere* obj_parse_sphere(obj_growable_scene_data *scene) {
  int temp_indices[MAX_VERTEX_COUNT];

  obj_sphere *obj = (obj_sphere*)malloc(sizeof(obj_sphere));
  obj_parse_vertex_index(temp_indices, obj->texture_index, NULL);
  obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, obj->texture_index);
  obj->pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
  obj->up_normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);
  obj->equator_normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[2]);

  return obj;
}

obj_plane* obj_parse_plane(obj_growable_scene_data *scene) {
  int temp_indices[MAX_VERTEX_COUNT];

  obj_plane *obj = (obj_plane*)malloc(sizeof(obj_plane));
  obj_parse_vertex_index(temp_indices, obj->texture_index, NULL);
  obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, obj->texture_index);
  obj->pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
  obj->normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);
  obj->rotation_normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[2]);

  return obj;
}

obj_light_point* obj_parse_light_point(obj_growable_scene_data *scene) {
  obj_light_point *o= (obj_light_point*)malloc(sizeof(obj_light_point));
  o->pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, atoi(strtok(NULL, OBJ_WHITESPACE)));
  return o;
}

obj_light_quad* obj_parse_light_quad(obj_growable_scene_data *scene) {
  obj_light_quad *o = (obj_light_quad*)malloc(sizeof(obj_light_quad));
  obj_parse_vertex_index(o->vertex_index, NULL, NULL);
  obj_convert_to_list_index_v(scene->vertex_list.item_count, o->vertex_index);

  return o;
}

obj_light_disc* obj_parse_light_disc(obj_growable_scene_data *scene) {
  int temp_indices[MAX_VERTEX_COUNT];

  obj_light_disc *obj = (obj_light_disc*)malloc(sizeof(obj_light_disc));
  obj_parse_vertex_index(temp_indices, NULL, NULL);
  obj->pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
  obj->normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);

  return obj;
}

obj_vector* obj_parse_vector3() {
  obj_vector *v = (obj_vector*)malloc(sizeof(obj_vector));
  v->e[0] = atof(strtok(NULL, OBJ_WHITESPACE));
  v->e[1] = atof(strtok(NULL, OBJ_WHITESPACE));
  v->e[2] = atof(strtok(NULL, OBJ_WHITESPACE));
  return v;
}


obj_vector2* obj_parse_vector2() {
  obj_vector2 *v = (obj_vector2*)malloc(sizeof(obj_vector2));
  v->e[0] = atof(strtok(NULL, OBJ_WHITESPACE));
  v->e[1] = atof(strtok(NULL, OBJ_WHITESPACE));
  return v;
}

void obj_parse_camera(obj_growable_scene_data *scene, obj_camera *camera) {
  int indices[3];
  obj_parse_vertex_index(indices, NULL, NULL);
  camera->camera_pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, indices[0]);
  camera->camera_look_point_index = obj_convert_to_list_index(scene->vertex_list.item_count, indices[1]);
  camera->camera_up_norm_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, indices[2]);
}

int obj_parse_mtl_file(char *filename, list *material_list) {
  int line_number = 0;
  char *current_token;
  char current_line[OBJ_LINE_MAX];
  char material_open = 0;
  obj_material *current_mtl = NULL;

  char *resolved_filename = realpath(filename, NULL);

  if (resolved_filename == NULL) {
    fprintf(stderr, "Error reading file %s: %s\n", filename, strerror(errno));
    return -1;
  }

  FILE *mtl_file_stream = fopen(resolved_filename, "r");
  if (mtl_file_stream == NULL) {
    fprintf(stderr, "Error reading file %s: %s\n", resolved_filename, strerror(errno));
    return -1;
  }

  list_make(material_list, 10, 1);

  while (fgets(current_line, OBJ_LINE_MAX, mtl_file_stream)) {
    current_token = strtok(current_line, OBJ_WHITESPACE);
    line_number++;

    if (current_token == NULL || strcmp(current_token, "//") == 0 || strcmp(current_token, "#") == 0) {
      //skip comments
      continue;
    } else if (strcmp(current_token, "newmtl") == 0) {
      material_open = 1;
      current_mtl = (obj_material*)malloc(sizeof(obj_material));
      obj_set_material_defaults(current_mtl);

      // get the name
      strncpy(current_mtl->name, strtok(NULL, MTL_WHITESPACE), NAME_MAX);
      list_add_item(material_list, current_mtl, current_mtl->name);
    } else if (strcmp(current_token, "Ka") == 0 && material_open) {
      // ambient
      current_mtl->amb[0] = atof(strtok(NULL, MTL_WHITESPACE));
      current_mtl->amb[1] = atof(strtok(NULL, MTL_WHITESPACE));
      current_mtl->amb[2] = atof(strtok(NULL, MTL_WHITESPACE));
    } else if (strcmp(current_token, "Ke") == 0 && material_open) {
      // emissive coeficient
    } else if (strcmp(current_token, "Kd") == 0 && material_open) {
      // diffuse
      current_mtl->diff[0] = atof(strtok(NULL, MTL_WHITESPACE));
      current_mtl->diff[1] = atof(strtok(NULL, MTL_WHITESPACE));
      current_mtl->diff[2] = atof(strtok(NULL, MTL_WHITESPACE));
    } else if (strcmp(current_token, "Ks") == 0 && material_open) {
      // specular
      current_mtl->spec[0] = atof(strtok(NULL, MTL_WHITESPACE));
      current_mtl->spec[1] = atof(strtok(NULL, MTL_WHITESPACE));
      current_mtl->spec[2] = atof(strtok(NULL, MTL_WHITESPACE));
    } else if (strcmp(current_token, "Ns") == 0 && material_open) {
      // shiny
      current_mtl->shiny = atof(strtok(NULL, MTL_WHITESPACE));
    } else if (strcmp(current_token, "d") == 0 && material_open) {
      // transparent
      current_mtl->trans = atof(strtok(NULL, MTL_WHITESPACE));
    } else if (strcmp(current_token, "r") == 0 && material_open) {
      // reflection
      current_mtl->reflect = atof(strtok(NULL, MTL_WHITESPACE));
    } else if (strcmp(current_token, "sharpness") == 0 && material_open) {
      // glossy
      current_mtl->glossy = atof(strtok(NULL, MTL_WHITESPACE));
    } else if (strcmp(current_token, "Ni") == 0 && material_open) {
      // refract index
      current_mtl->refract_index = atof(strtok(NULL, MTL_WHITESPACE));
    } else if (strcmp(current_token, "illum") == 0 && material_open) {
      // illumination type
    } else if (strcmp(current_token, "map_Ka") == 0 && material_open) {
      // ambient texture map
      strncpy(current_mtl->amb_texture_filename, strtok(NULL, MTL_WHITESPACE), PATH_MAX);
    } else if(strcmp(current_token, "map_Kd") == 0 && material_open) {
      // diffuse texture map
      strncpy(current_mtl->diff_texture_filename, strtok(NULL, MTL_WHITESPACE), PATH_MAX);
    } else if(strcmp(current_token, "map_Ke") == 0 && material_open) {
      // map emissive coefficient
    } else if(strcmp(current_token, "map_d") == 0 && material_open) {
      // alpha texture map
    } else {
      fprintf(stderr, "Unknown command '%s' in material file %s at line %i:\n\t%s\n", current_token, filename, line_number, current_line);
      fclose(mtl_file_stream);
      return -1;
    }
  }

  fclose(mtl_file_stream);
  return line_number;
}

int obj_parse_obj_file(obj_growable_scene_data *growable_data, const char *filename) {
  int current_material = -1;
  char *current_token = NULL;
  char current_line[OBJ_LINE_MAX];
  int line_number = 0;

  char *resolved_filename = realpath(filename, NULL);

  if (resolved_filename == NULL) {
    fprintf(stderr, "Error reading file %s: %s\n", filename, strerror(errno));
    return -1;
  }

  FILE* obj_file_stream = fopen(resolved_filename, "r");
  if (obj_file_stream == NULL) {
    fprintf(stderr, "Error reading file %s: %s\n", resolved_filename, strerror(errno));
    return -1;
  }

  //parser loop
  while (fgets(current_line, OBJ_LINE_MAX, obj_file_stream)) {
    current_token = strtok(current_line, " \t\n\r");
    line_number++;

    if (current_token == NULL || current_token[0] == '#') {
      //skip comments
      continue;
    } else if (strcmp(current_token, "v") == 0) {
      // process vertex
      list_add_item(&growable_data->vertex_list, obj_parse_vector3(), NULL);
    } else if (strcmp(current_token, "vn") == 0) {
      // process vertex normal
      list_add_item(&growable_data->vertex_normal_list, obj_parse_vector3(), NULL);
    } else if (strcmp(current_token, "vt") == 0) {
      // process vertex texture
      list_add_item(&growable_data->vertex_texture_list, obj_parse_vector2(), NULL);
    } else if (strcmp(current_token, "f") == 0) {
      // process face
      obj_face *face = obj_parse_face(growable_data);
      face->material_index = current_material;
      list_add_item(&growable_data->face_list, face, NULL);
    } else if (strcmp(current_token, "sp") == 0) {
      // process sphere
      obj_sphere *sphr = obj_parse_sphere(growable_data);
      sphr->material_index = current_material;
      list_add_item(&growable_data->sphere_list, sphr, NULL);
    } else if (strcmp(current_token, "pl") == 0) {
      // process plane
      obj_plane *pl = obj_parse_plane(growable_data);
      pl->material_index = current_material;
      list_add_item(&growable_data->plane_list, pl, NULL);
    } else if (strcmp(current_token, "p") == 0) {
      // process point
      //make a small sphere to represent the point?
    } else if (strcmp(current_token, "lp") == 0) {
      // light point source
      obj_light_point *o = obj_parse_light_point(growable_data);
      o->material_index = current_material;
      list_add_item(&growable_data->light_point_list, o, NULL);
    } else if (strcmp(current_token, "ld") == 0) {
      // process light disc
      obj_light_disc *o = obj_parse_light_disc(growable_data);
      o->material_index = current_material;
      list_add_item(&growable_data->light_disc_list, o, NULL);
    } else if (strcmp(current_token, "lq") == 0) {
      // process light quad
      obj_light_quad *o = obj_parse_light_quad(growable_data);
      o->material_index = current_material;
      list_add_item(&growable_data->light_quad_list, o, NULL);
    } else if (strcmp(current_token, "c") == 0) {
      // camera
      growable_data->camera = (obj_camera*)malloc(sizeof(obj_camera));
      obj_parse_camera(growable_data, growable_data->camera);
    } else if (strcmp(current_token, "usemtl") == 0) {
      // usemtl
      current_material = list_find(&growable_data->material_list, strtok(NULL, OBJ_WHITESPACE));
    } else if (strcmp(current_token, "mtllib") == 0) {
      // mtllib
      strncpy(growable_data->material_filename, strtok(NULL, OBJ_WHITESPACE), PATH_MAX);

      char mtl_file_path[PATH_MAX];
      if (growable_data->material_filename[0] == '/') {
        // absolute path
        strcpy(mtl_file_path, growable_data->material_filename);
      } else {
        // relative path
        char* filename_dup = alloca(strlen(filename) + 1);
        strcpy(filename_dup, filename);
        char* mtl_file_dirname = dirname(filename_dup);
        sprintf(mtl_file_path, "%s/%s", mtl_file_dirname, growable_data->material_filename);
      }

      obj_parse_mtl_file(mtl_file_path, &growable_data->material_list);
    } else if (strcmp(current_token, "o") == 0) {
      // object name
    } else if (strcmp(current_token, "s") == 0) {
      // smoothing
    } else if (strcmp(current_token, "g") == 0) {
      // group
    } else {
      printf("Unknown command '%s' in scene code at line %i: \"%s\".\n", current_token, line_number, current_line);
    }
  }

  fclose(obj_file_stream);
  return line_number;
}


void obj_init_temp_storage(obj_growable_scene_data *growable_data) {
  list_make(&growable_data->vertex_list, 10, 1);
  list_make(&growable_data->vertex_normal_list, 10, 1);
  list_make(&growable_data->vertex_texture_list, 10, 1);

  list_make(&growable_data->face_list, 10, 1);
  list_make(&growable_data->sphere_list, 10, 1);
  list_make(&growable_data->plane_list, 10, 1);

  list_make(&growable_data->light_point_list, 10, 1);
  list_make(&growable_data->light_quad_list, 10, 1);
  list_make(&growable_data->light_disc_list, 10, 1);

  list_make(&growable_data->material_list, 10, 1);

  growable_data->camera = NULL;
}

void obj_free_temp_storage(obj_growable_scene_data *growable_data) {
  obj_free_half_list(&growable_data->vertex_list);
  obj_free_half_list(&growable_data->vertex_normal_list);
  obj_free_half_list(&growable_data->vertex_texture_list);

  obj_free_half_list(&growable_data->face_list);
  obj_free_half_list(&growable_data->sphere_list);
  obj_free_half_list(&growable_data->plane_list);

  obj_free_half_list(&growable_data->light_point_list);
  obj_free_half_list(&growable_data->light_quad_list);
  obj_free_half_list(&growable_data->light_disc_list);

  obj_free_half_list(&growable_data->material_list);
}

void delete_obj_scene_data(obj_scene_data *data_out) {
  FREE_ELEMENT(data_out->vertex_list, data_out->vertex_count);
  FREE_ELEMENT(data_out->vertex_normal_list, data_out->vertex_normal_count);
  FREE_ELEMENT(data_out->vertex_texture_list, data_out->vertex_texture_count);
  FREE_ELEMENT(data_out->face_list, data_out->face_count);
  FREE_ELEMENT(data_out->sphere_list, data_out->sphere_count);
  FREE_ELEMENT(data_out->plane_list, data_out->plane_count);
  FREE_ELEMENT(data_out->light_point_list, data_out->light_point_count);
  FREE_ELEMENT(data_out->light_disc_list, data_out->light_disc_count);
  FREE_ELEMENT(data_out->light_quad_list, data_out->light_quad_count);
  FREE_ELEMENT(data_out->material_list, data_out->material_count);

  free(data_out->camera);
}

void obj_copy_to_out_storage(obj_scene_data *data_out, obj_growable_scene_data *growable_data) {
  data_out->vertex_count = growable_data->vertex_list.item_count;
  data_out->vertex_normal_count = growable_data->vertex_normal_list.item_count;
  data_out->vertex_texture_count = growable_data->vertex_texture_list.item_count;

  data_out->face_count = growable_data->face_list.item_count;
  data_out->sphere_count = growable_data->sphere_list.item_count;
  data_out->plane_count = growable_data->plane_list.item_count;

  data_out->light_point_count = growable_data->light_point_list.item_count;
  data_out->light_disc_count = growable_data->light_disc_list.item_count;
  data_out->light_quad_count = growable_data->light_quad_list.item_count;

  data_out->material_count = growable_data->material_list.item_count;

  data_out->vertex_list = (obj_vector**)growable_data->vertex_list.items;
  data_out->vertex_normal_list = (obj_vector**)growable_data->vertex_normal_list.items;
  data_out->vertex_texture_list = (obj_vector2**)growable_data->vertex_texture_list.items;

  data_out->face_list = (obj_face**)growable_data->face_list.items;
  data_out->sphere_list = (obj_sphere**)growable_data->sphere_list.items;
  data_out->plane_list = (obj_plane**)growable_data->plane_list.items;

  data_out->light_point_list = (obj_light_point**)growable_data->light_point_list.items;
  data_out->light_disc_list = (obj_light_disc**)growable_data->light_disc_list.items;
  data_out->light_quad_list = (obj_light_quad**)growable_data->light_quad_list.items;

  data_out->material_list = (obj_material**)growable_data->material_list.items;

  data_out->camera = growable_data->camera;
}

int parse_obj_scene_data(obj_scene_data *data_out, const char *filename) {
  obj_growable_scene_data growable_data;

  obj_init_temp_storage(&growable_data);
  int parsed_lines = obj_parse_obj_file(&growable_data, filename);
  if (parsed_lines == -1) {
    return -1;
  }

  obj_copy_to_out_storage(data_out, &growable_data);
  obj_free_temp_storage(&growable_data);
  return parsed_lines;
}
