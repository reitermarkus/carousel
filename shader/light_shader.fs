#version 330

struct vertex_data {
  vec3 position;
  vec4 color;
  vec3 normal;
  vec2 texture;
  vec4 eye_space_position;
};

struct light_data {
  vec3 position;
  vec3 color;
};

uniform mat4 view_matrix;

in vertex_data vertex;

out vec4 fragment_color;

void main() {
  fragment_color = vertex.color;
}
