#version 330

struct vector_data {
  vec3 position;
  vec4 color;
  vec3 normal;
  vec2 texture;
};

struct light_data {
  vec3 position;
  vec3 color;
};

uniform mat4 view_matrix;

in vector_data vector;

#define MAX_LIGHTS 10
uniform int light_count;
uniform light_data lights[MAX_LIGHTS];

uniform float diffuse_factor;
uniform float specular_factor;
uniform float ambient_factor;

out vec4 fragment_color;

uniform sampler2D texture_sampler;

void light(light_data light, vector_data vector, out vec3 ambient, out vec3 diffuse, out vec3 specular) {
  vec3 normal = normalize(vector.normal);

  vec3 light_position = vec3(view_matrix * vec4(light.position, 1.0));
  vec3 light_direction = normalize(light_position - vector.position);
  vec3 view_direction = normalize(-vector.position);
  vec3 reflect_direction = reflect(-light_direction, normal);

  ambient = vec3(ambient_factor) * light.color;
  diffuse = vec3(diffuse_factor * max(dot(normal, light_direction), 0.0)) * light.color;
  float specular_shininess = 32;
  specular = vec3(specular_factor * pow(max(dot(reflect_direction, view_direction), 0.0), specular_shininess));
}

void main() {
  vec3 ambient_sum = vec3(0);
  vec3 diffuse_sum = vec3(0);
  vec3 spec_sum = vec3(0);

  for (int i = 0; i < light_count; i++) {
    vec3 ambient, diffuse, specular;

    light(lights[i], vector, ambient, diffuse, specular);

    ambient_sum += ambient;
    diffuse_sum += diffuse;
    spec_sum += specular;
  }

  ambient_sum /= light_count;

  vec4 texture_color = texture(texture_sampler, vector.texture);

  fragment_color = vector.color * vec4(ambient_sum + diffuse_sum + spec_sum, 1) * texture_color;
}
