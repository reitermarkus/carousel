#include "init_external_obj.h"

#include <stdio.h>

#include "parser/obj_parser.h"

void init_external_obj(struct object_data* obj, char* filename) {
  obj_scene_data ext_obj;

  if (parse_obj_scene_data(&ext_obj, filename) == -1) {
    printf("Could not load file. Exiting.\n");
    exit(EXIT_FAILURE);
  }

  // Copy mesh data from structs into appropriate arrays.

  obj->vertex_count = ext_obj.vertex_texture_count;
  obj->index_count = ext_obj.face_count;
  obj->texture_count = ext_obj.vertex_texture_count;

  obj->vertices = malloc(obj->texture_count * sizeof(*obj->vertices));
  obj->indices = malloc(obj->index_count * sizeof(*obj->indices));
  obj->textures = malloc(obj->texture_count * sizeof(*obj->textures));

  for (size_t i = 0; i < obj->vertex_count; i++) {
    SET_VERTEX_COLOR(obj->vertices[i], 1.0, 1.0, 1.0, 1.0);
  }

  for (int i = 0; i < ext_obj.face_count; i++) {
    // Vertices
    int index0 = ext_obj.face_list[i]->vertex_index[0];
    int index1 = ext_obj.face_list[i]->vertex_index[1];
    int index2 = ext_obj.face_list[i]->vertex_index[2];

    // Indices
    obj->indices[i].a = ext_obj.face_list[i]->texture_index[0];
    obj->indices[i].b = ext_obj.face_list[i]->texture_index[1];
    obj->indices[i].c = ext_obj.face_list[i]->texture_index[2];

    SET_VERTEX_POSITION(obj->vertices[obj->indices[i].a],
      ext_obj.vertex_list[index0]->e[0],
      ext_obj.vertex_list[index0]->e[1],
      ext_obj.vertex_list[index0]->e[2]
    );

    SET_VERTEX_POSITION(obj->vertices[obj->indices[i].b],
      ext_obj.vertex_list[index1]->e[0],
      ext_obj.vertex_list[index1]->e[1],
      ext_obj.vertex_list[index1]->e[2]
    );

    SET_VERTEX_POSITION(obj->vertices[obj->indices[i].c],
      ext_obj.vertex_list[index2]->e[0],
      ext_obj.vertex_list[index2]->e[1],
      ext_obj.vertex_list[index2]->e[2]
    );

    obj->textures[obj->indices[i].a].u = ext_obj.vertex_texture_list[obj->indices[i].a]->e[0];
    obj->textures[obj->indices[i].a].v = ext_obj.vertex_texture_list[obj->indices[i].a]->e[1];
    obj->textures[obj->indices[i].b].u = ext_obj.vertex_texture_list[obj->indices[i].b]->e[0];
    obj->textures[obj->indices[i].b].v = ext_obj.vertex_texture_list[obj->indices[i].b]->e[1];
    obj->textures[obj->indices[i].c].u = ext_obj.vertex_texture_list[obj->indices[i].c]->e[0];
    obj->textures[obj->indices[i].c].v = ext_obj.vertex_texture_list[obj->indices[i].c]->e[1];
  }

  delete_obj_scene_data(&ext_obj);
}
