#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

uniform vec3 light_position_1;
uniform vec3 light_position_2;
uniform vec3 light_color_1;
uniform vec3 light_color_2;
uniform float diffuse_factor;
uniform float specular_factor;
uniform float ambient_factor;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texture;
layout (location = 3) in vec3 normal;

out vec4 v_color;
out vec2 v_texture;

void main() {
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);

	mat4 normal_matrix = transpose(inverse(view_matrix * model_matrix));
	vec3 normal = normalize((normal_matrix * vec4(normalize(normal), 1.0)).xyz);
	
	
	vec3 light_position_1 = (view_matrix * vec4(light_position_1, 1.0)).xyz;
	vec3 light_position_2 = (view_matrix * vec4(light_position_2, 1.0)).xyz;
	vec4 vertex_position_model_space = (view_matrix * model_matrix) * vec4(position,1.0);
	vec3 vertex_normalized = normalize(-vertex_position_model_space.xyz);
	
	vec3 light_vector_1 = normalize(light_position_1 - vertex_position_model_space.xyz);
	vec3 light_vector_2 = normalize(light_position_2 - vertex_position_model_space.xyz);
	vec3 half_vector_1 = normalize(light_vector_1 + vertex_normalized);
	vec3 half_vector_2 = normalize(light_vector_2 + vertex_normalized);   
	
	vec3 diffuse_part = (clamp(dot(normal,light_vector_1), 0.0, 1.0)*light_color_1 + clamp(dot(normal,light_vector_2), 0.0, 1.0)*light_color_2);
	vec3 specular_part = (pow(clamp(dot(normal,half_vector_1),0.0,1.0),127.0 )*light_color_1 + pow(clamp(dot(normal,half_vector_2),0.0,1.0 ),127.0)*light_color_2);
	vec3 ambient_part = vec3(color * ambient_factor);
	diffuse_part *= vec3(diffuse_factor);
	specular_part *= vec3(specular_factor);
	
	
  v_color = vec4(color.r, color.g, color.b, color.a);
  v_texture = texture;
}
