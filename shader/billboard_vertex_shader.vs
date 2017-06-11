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

void main()
{
    mat4 model_view = model_matrix * view_matrix;

    model_view[0][0] = 1; 
    model_view[0][2] = 0; 

    model_view[2][0] = 0; 
    model_view[2][2] = 1; 

    vec4 projection_model_view_positon = projection_matrix * model_view * vec4(position, 1.0);

    gl_Position = projection_model_view_positon;

    vector.position = vec3(projection_model_view_positon);
    vector.color = color;
    vector.normal = mat3(transpose(inverse(view_matrix * model_matrix))) * normal;
    vector.texture = texture;
}
