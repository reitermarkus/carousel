#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texture;
layout (location = 3) in vec3 normal;

struct vector_data {
  vec3 position;
  vec4 color;
  vec3 normal;
  vec2 texture;
};

out vector_data vector;

void main() {
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);

  vector.position = vec3(view_matrix * model_matrix * vec4(position, 1.0));
  vector.color = color;
  vector.normal = mat3(transpose(inverse(view_matrix * model_matrix))) * normal;
  vector.texture = texture;
}
