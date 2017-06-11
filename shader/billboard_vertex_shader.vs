#version 330

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

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

    model_view[0][0] = 1.0; 
    model_view[0][1] = 0.0; 
    model_view[0][2] = 0.0; 

    model_view[2][0] = 0.0; 
    model_view[2][1] = 0.0; 
    model_view[2][2] = 1.0; 

    vec4 model_view_position = model_view * vec4(position, 1.0);

    gl_Position = projection_matrix * model_view_position;

    vector.texture = texture;
    vector.color = color;
}
