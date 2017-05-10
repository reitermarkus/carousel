#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texture;
layout (location = 3) in vec3 normal;

out vec3 v_position;
out vec4 v_color;
out vec2 v_texture;
out vec3 v_normal;

uniform vec3 light_position_1;
out vec3 light_pos_1;

void main() {
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);

  v_position = vec3(view_matrix * model_matrix * vec4(position, 1));
  v_color = color;
  v_normal = mat3(transpose(inverse(view_matrix * model_matrix))) * normal;
  v_texture = texture;

  light_pos_1 = vec3(view_matrix * vec4(light_position_1, 1.0));
}
