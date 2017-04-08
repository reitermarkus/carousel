#ifndef __CREATE_SHADER_PROGRAM_H__
#define __CREATE_SHADER_PROGRAM_H__

#include "helper/shared_headers.h"

// Creates and adds a shader to a shader program.
void add_shader(GLuint shader_program, const char* shader_code, GLenum shader_type);

// Creates and enables a shader program with the given vertex and fragment shader files.
GLuint create_shader_program(const char* vertex_shader, const char* fragment_shader);

#endif // __CREATE_SHADER_PROGRAM_H__
