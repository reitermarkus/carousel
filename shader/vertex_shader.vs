#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texture;
layout (location = 3) in vec3 normal;

struct vertex_data {
  vec3 position;
  vec4 color;
  vec3 normal;
  vec2 texture;
  vec4 eye_space_position;
};

out vertex_data vertex;

void main() {
  vec4 eye_space_position = view_matrix * model_matrix * vec4(position, 1.0);

  gl_Position = projection_matrix * eye_space_position;

  vertex.position = vec3(view_matrix * model_matrix * vec4(position, 1.0));
  vertex.color = color;
  vertex.normal = mat3(transpose(inverse(view_matrix * model_matrix))) * normal;
  vertex.texture = texture;

  vertex.eye_space_position = eye_space_position;
}
