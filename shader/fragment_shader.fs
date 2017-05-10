#version 330

in vec3 v_position;
in vec4 v_color;
in vec3 v_normal;
in vec2 v_texture;
in vec3 light_pos_1;

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

  vec3 normal = normalize(v_normal);
  vec3 light_direction = normalize(light_pos_1 - v_position);
  vec3 view_direction = normalize(-v_position);
  vec3 reflect_direction = reflect(-light_direction, normal);

  vec3 ambient = vec3(ambient_factor);
  vec3 diffuse = vec3(diffuse_factor) * max(dot(normal, light_direction), 0.0);
  vec3 specular = vec3(specular_factor) * pow(max(dot(reflect_direction, view_direction), 0.0), 100);

  ambient_sum += ambient;
  diffuse_sum += diffuse;
  spec_sum += specular;

  ambient_sum /= 1; // 1 := light count

  vec4 texture_color = texture(tiger_texture, v_texture);

  fragment_color = v_color * vec4(ambient_sum + ((diffuse_sum + spec_sum) * light_color_1), 1) /* * texture_color */;
}
