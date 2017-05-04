#version 330

in vec2 v_texture;

uniform sampler2D tiger_texture;

out vec4 color;

void main() {
  color = texture(tiger_texture, v_texture);
}
