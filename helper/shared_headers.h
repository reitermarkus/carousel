#ifndef __H_SHARED_HEADERS__
#define __H_SHARED_HEADERS__

#if __APPLE__
  #include <TargetConditionals.h>
  #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
  #include <OpenGL/gl3.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/freeglut.h>
#endif

#endif
