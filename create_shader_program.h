#ifndef __H_CREATE_SHADER_PROGRAM__
#define __H_CREATE_SHADER_PROGRAM__

void add_shader(GLuint shader_program, const char* shader_code, GLenum shader_type);
GLuint create_shader_program(const char* vertexshader, const char* fragmentshader);

#endif
