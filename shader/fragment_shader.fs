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

struct fog_data {
  int   equation;
  vec4  color;
  float start;
  float end;
  float density;
};

uniform mat4 view_matrix;
uniform mat4 model_matrix;

in vertex_data vertex;

#define MAX_LIGHTS 10
uniform int light_count;
uniform light_data lights[MAX_LIGHTS];

uniform float diffuse_factor;
uniform float specular_factor;
uniform float ambient_factor;

uniform fog_data fog;

out vec4 fragment_color;

uniform bool texture_enabled;
uniform sampler2D texture_sampler;

void light(light_data light, vertex_data vertex, out vec3 ambient, out vec3 diffuse, out vec3 specular) {
  vec3 normal = normalize(vertex.normal);

  vec3 light_position = vec3(view_matrix * vec4(light.position, 1.0));
  vec3 light_direction = normalize(light_position - vertex.position);
  vec3 view_direction = normalize(-vertex.position);
  vec3 reflect_direction = reflect(-light_direction, normal);

  ambient = vec3(ambient_factor) * light.color;
  diffuse = vec3(diffuse_factor * max(dot(normal, light_direction), 0.0)) * light.color;
  float specular_shininess = 32;
  specular = vec3(specular_factor * pow(max(dot(reflect_direction, view_direction), 0.0), specular_shininess));
}

float getFogFactor(fog_data fog, float fog_coordinates) {
	float result = 0.0;

	if(fog.equation == 0) {
		result = (fog.end - fog_coordinates) / (fog.end - fog.start);
  } else if(fog.equation == 1) {
		result = exp(-fog.density * fog_coordinates);
  } else if(fog.equation == 2) {
	  result = exp(-pow(fog.density * fog_coordinates, 2.0));
  }

	result = 1.0 - clamp(result, 0.0, 1.0);

	return result;
}

void main() {
  vec3 ambient_sum = vec3(0);
  vec3 diffuse_sum = vec3(0);
  vec3 spec_sum = vec3(0);

  for (int i = 0; i < light_count; i++) {
    vec3 ambient, diffuse, specular;

    light(lights[i], vertex, ambient, diffuse, specular);

    ambient_sum += ambient;
    diffuse_sum += diffuse;
    spec_sum += specular;
  }

  ambient_sum /= light_count;

  vec4 texture_color = vec4(1);

  if (texture_enabled) {
    texture_color = texture(texture_sampler, vertex.texture);
  }

  fragment_color = vertex.color * vec4(ambient_sum + diffuse_sum + spec_sum, 1) * texture_color;

  float fog_coordinates = abs(vertex.eye_space_position.z / vertex.eye_space_position.w);
  fragment_color = mix(fragment_color, fog.color, getFogFactor(fog, fog_coordinates));
}
