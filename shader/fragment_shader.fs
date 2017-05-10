#version 330

in vec3 v_position;
in vec4 v_color;
in vec3 v_normal;
in vec2 v_texture;

uniform vec3 light_position_1;
uniform vec3 light_position_2;
uniform vec3 light_color_1;
uniform vec3 light_color_2;
uniform float diffuse_factor;
uniform float specular_factor;
uniform float ambient_factor;

out vec4 fragment_color;

uniform sampler2D tiger_texture;

void main() {
  vec3 ambient_sum = vec3(0);
  vec3 diffuse_sum = vec3(0);
  vec3 spec_sum = vec3(0);

  vec3 n = normalize(v_normal);
  vec3 s = normalize(light_position_1 - v_position);
  vec3 v = normalize(-v_position);
  vec3 r = reflect(-s, n);

  vec3 ambient = vec3(ambient_factor);
  vec3 diffuse = vec3(diffuse_factor) * max(dot(s, n), 0.0);
  vec3 specular = vec3(specular_factor) * max(dot(r, v), 0.0);

  ambient_sum += ambient;
  diffuse_sum += diffuse;
  spec_sum += specular;

  ambient_sum /= 1; // 1 := light count

  vec4 texture_color = texture(tiger_texture, v_texture);

  fragment_color = v_color * vec4(ambient_sum, 1) /* * texture_color */ + vec4((diffuse_sum + spec_sum) * light_color_1, 0);
}
