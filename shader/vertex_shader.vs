#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texture;

out vec4 v_color;
out vec2 v_texture;

void main() {
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);
  v_color = vec4(color.r, color.g, color.b, color.a);
  v_texture = texture;
}
