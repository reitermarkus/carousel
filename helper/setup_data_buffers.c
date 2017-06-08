#include "setup_data_buffers.h"

void setup_data_buffers(struct object_data* object) {
  if (object->face_normals == NULL) {
    calculate_normals(object);
  }

  glGenVertexArrays(1, &(object->vao));
  glBindVertexArray(object->vao);

  // Bind buffer object for vertices and colors.
  glGenBuffers(1, &(object->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, object->vertex_count * sizeof(*object->vertices), object->vertices, GL_STATIC_DRAW);

  // Bind buffer object for indices.
  glGenBuffers(1, &(object->ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->index_count * sizeof(*object->indices), object->indices, GL_STATIC_DRAW);

  // Bind vertex positions.
  glEnableVertexAttribArray(v_position);
  glVertexAttribPointer(
    v_position,            // attribute index
    3,                     // attribute length (x, y, z)
    GL_FLOAT,              // attribute type
    GL_FALSE,              // normalized?
    sizeof(struct vertex), // offset between indices
    0                      // offset to vertex values
  );

  // Bind vertex colors.
  glEnableVertexAttribArray(v_color);
  glVertexAttribPointer(
    v_color,                         // attribute index
    4,                               // attribute length (r, g, b, a)
    GL_FLOAT,                        // attribute type
    GL_FALSE,                        // normalized?
    sizeof(struct vertex),           // offset between indices
    (GLvoid*)sizeof(struct position) // offset to color values
  );

  glEnableVertexAttribArray(v_normal);
  glVertexAttribPointer(
    v_normal,                                                 // attribute index
    3,                                                        // attribute length (r, g, b, a)
    GL_FLOAT,                                                 // attribute type
    GL_FALSE,                                                 // normalized?
    sizeof(struct vertex),                                    // offset between indices
    (GLvoid*)(sizeof(struct position) + sizeof(struct color)) // offset to color values
  );

  if (object->texture_count != 0) {
    glGenBuffers(1, &(object->tbo));
    glBindBuffer(GL_ARRAY_BUFFER, object->tbo);
    glBufferData(GL_ARRAY_BUFFER, object->texture_count * sizeof(*object->textures), object->textures, GL_STATIC_DRAW);

    // Bind vertex textures.
    glEnableVertexAttribArray(v_texture);
     glVertexAttribPointer(
       v_texture,                                                // attribute index
       2,                                                        // attribute length (u, v)
       GL_FLOAT,                                                 // attribute type
       GL_FALSE,                                                 // normalized?
       0,                                                        // offset between indices
       0                                                         // offset to texture values
     );
  }

  // Bind indices.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);

  glBindVertexArray(0);

  glDisableVertexAttribArray(v_position);
  glDisableVertexAttribArray(v_color);
  glDisableVertexAttribArray(v_normal);
  glDisableVertexAttribArray(v_texture);
}
