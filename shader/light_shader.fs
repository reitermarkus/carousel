#version 330

struct vector_data {
  vec3 position;
  vec4 color;
  vec3 normal;
  vec2 texture;
};

in vector_data vector;

out vec4 fragment_color;

void main() {
  fragment_color = vector.color;
}
