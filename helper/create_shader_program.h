#ifndef __CREATE_SHADER_PROGRAM_H__
#define __CREATE_SHADER_PROGRAM_H__

#include "helper/shared_headers.h"

void add_shader(GLuint shader_program, const char* shader_code, GLenum shader_type);
GLuint create_shader_program(const char* vertexshader, const char* fragmentshader);

#endif
