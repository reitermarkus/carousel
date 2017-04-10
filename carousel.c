/******************************************************************
*
* carousel.c
*
* Description: This example demonstrates a colored, rotating
* cube in shader-based OpenGL. The use of transformation
* matrices, perspective projection, and indexed triangle sets
* are shown.
*
* Computer Graphics Proseminar SS 2017
*
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/


/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "helper/shared_headers.h"

/* Local includes */
#include "helper/matrix.h"
#include "helper/macros.h"
#include "helper/create_shader_program.h"
#include "helper/draw.h"
#include "shape/cone.h"
#include "shape/cube.h"
#include "shape/cylinder.h"
#include "shape/polygon.h"
#include "helper/vertex.h"
#include "parser/obj_parser.h"


/*----------------------------------------------------------------*/

static matrix projection_matrix;
static matrix view_matrix;

static float camera_translation_x = 0.0;
static float camera_translation_y = 0.0;
static float camera_rotation_y = 0.0;
static float camera_translation_z = 0.0;
static const float camera_speed = 0.1;

enum { number_of_sides = 8 };
static struct object_data base;
static struct object_data center_pillar;
static struct object_data roof;
static struct object_data pillars[number_of_sides];
static struct object_data cubes[number_of_sides];


/* Structures for loading of OBJ data */
obj_scene_data wolf;

static const float PILLAR_HEIGHT = 1.5;
static const float BASE_HEIGHT = .15;
static const float BASE_RADIUS = 2.5;
static const float ROOF_HEIGHT = 1.0;
static const float CENTER_PILLAR_RADIUS = 0.5;

/*----------------------------------------------------------------*/

/******************************************************************
*
* setup_data_buffers
*
* Create buffer objects and load data into buffers
*
*******************************************************************/

void setup_data_buffers(struct object_data* object) {
  // for MAC
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Bind buffer object for vertices and colors.
  glGenBuffers(1, &(object->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, object->vertices_size, object->vertices, GL_STATIC_DRAW);
  free(object->vertices);

  // Bind buffer object for indices.
  glGenBuffers(1, &(object->ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->indices_size, object->indices, GL_STATIC_DRAW);
  free(object->indices);
}

/******************************************************************
*
* Setup and Initialization for external ".obj" files
*
*******************************************************************/

void setup_data_buffer_ext_obj(obj_scene_data* obj){
  // for MAC
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &(obj->obj_data.vbo));
  glBindBuffer(GL_ARRAY_BUFFER, obj->obj_data.vbo);
  glBufferData(GL_ARRAY_BUFFER, obj->vertex_count*3*sizeof(GLfloat), obj->vertex_buffer_data, GL_STATIC_DRAW);

  glGenBuffers(1, &(obj->obj_data.ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->obj_data.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->face_count*3*sizeof(GLushort), obj->index_buffer_data, GL_STATIC_DRAW);
}

void setup_shader_program_ext_obj(obj_scene_data* obj) {
  // Put linked shader program into drawing pipeline.
  obj->obj_data.shader_program = create_shader_program(obj->obj_data.vertex_shader_file,obj->obj_data.fragment_shader_file);
}

void init_ext_obj(obj_scene_data* obj, char* filename){
  if (parse_obj_scene(obj, filename) == -1) {
    printf("Could not load file. Exiting.\n");
    exit(EXIT_FAILURE);
  }

  //  Copy mesh data from structs into appropriate arrays.
  int vert = obj->vertex_count;
  int indx = obj->face_count;

  obj->vertex_buffer_data = (GLfloat*) calloc (vert*3, sizeof(GLfloat));
  obj->index_buffer_data = (GLushort*) calloc (indx*3, sizeof(GLushort));

  // Vertices
  for (int i = 0; i < vert; i++){
    obj->vertex_buffer_data[i*3] = (GLfloat)((obj->vertex_list[i])->e[0]);
    obj->vertex_buffer_data[i*3+1] = (GLfloat)((obj->vertex_list[i])->e[1]);
    obj->vertex_buffer_data[i*3+2] = (GLfloat)((obj->vertex_list[i])->e[2]);
  }

  // Indices
  for (int i = 0; i < indx; i++)  {
    obj->index_buffer_data[i*3] = (GLushort)(obj->face_list[i])->vertex_index[0];
    obj->index_buffer_data[i*3+1] = (GLushort)(obj->face_list[i])->vertex_index[1];
    obj->index_buffer_data[i*3+2] = (GLushort)(obj->face_list[i])->vertex_index[2];
  }

  setup_data_buffer_ext_obj(obj);
  setup_shader_program_ext_obj(obj);
}

/******************************************************************
*
* display
*
* This function is called when the content of the window needs to be
* drawn/redrawn. It has been specified through 'glutDisplayFunc()';
* Enable vertex attributes, create binding between C program and
* attribute name in shader
*
*******************************************************************/

void setup_shader_program(struct object_data* object) {
  // Put linked shader program into drawing pipeline.
  object->shader_program = create_shader_program(object->vertex_shader_file, object->fragment_shader_file);
}

void display_object(struct object_data* object) {
  draw(object, projection_matrix, view_matrix);
}

// Displays an external ".obj" object
void display_ext_object(obj_scene_data* obj){
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, obj->obj_data.vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->obj_data.ibo);

  GLint size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  /* Issue draw command, using indexed triangle list */
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  /* Set state to only draw wireframe (no lighting used, yet) */
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  /* Disable attributes */
  glDisableVertexAttribArray(0);
}

void display() {
  // Clear window with color specified in `initialize`.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrix_translate_x(camera_speed * camera_translation_x, view_matrix);
  matrix_translate_y(camera_speed * camera_translation_y, view_matrix);
  matrix_rotate_y(camera_speed / M_PI * camera_rotation_y, view_matrix);
  matrix_translate_z(camera_speed * camera_translation_z, view_matrix);

  display_ext_object(&wolf);
  display_object(&base);
  display_object(&center_pillar);
  display_object(&roof);

  for (int i = 0; i < number_of_sides; i++) {
    display_object(&(pillars[i]));
    display_object(&(cubes[i]));
  }

  // Swap between front and back buffer.
  glutSwapBuffers();
}


/******************************************************************
*
* on_idle
*
*
*
*******************************************************************/

void on_idle() {
  float rotations_per_second = 1.0f / 8;
  long elapsed_time = glutGet(GLUT_ELAPSED_TIME); // ms
  float angle = elapsed_time * rotations_per_second * deg_to_rad(360) / 1000; // rad

  // Rotate clock-wise by using negative angle.
  float rotation = -angle;

  // Initialize center pillar matrix.
  matrix_identity(center_pillar.translation_matrix);

  // Move center pillar onto the base.
  matrix_translate_y(BASE_HEIGHT, center_pillar.translation_matrix);

  // Rotate center pillar.
  matrix_rotate_y(rotation, center_pillar.translation_matrix);

  // Initialize base matrix.
  matrix_identity(base.translation_matrix);

  // Rotate base.
  matrix_rotate_y(rotation, base.translation_matrix);

  // Initialize roof matrix.
  matrix_identity(roof.translation_matrix);

  // Rotate roof.
  matrix_rotate_y(rotation, roof.translation_matrix);

  // Move roof up above the pillars.
  matrix_translate_y(PILLAR_HEIGHT + BASE_HEIGHT, roof.translation_matrix);

  for (int i = 0; i < number_of_sides; i++) {
    // Initialize pillar matrix.
    matrix_identity(pillars[i].translation_matrix);

    // Move pillar onto the base.
    matrix_translate_y(BASE_HEIGHT, pillars[i].translation_matrix);

    // Move pillar towards the edge.
    matrix_translate_z(-(BASE_RADIUS / 7 * 6), pillars[i].translation_matrix);

    // Rotate pillar around the center to the corresponding edge.
    matrix_rotate_y(deg_to_rad(360) / (float)number_of_sides * (float)i, pillars[i].translation_matrix);

    // Apply general rotation to pillar.
    matrix_rotate_y(rotation, pillars[i].translation_matrix);
  }

  for (int i=0; i < number_of_sides; i++){
    // Initialize cube matrix.
    matrix_identity(cubes[i].translation_matrix);

    // Move cube towards the edge.
    matrix_translate_z(-(BASE_RADIUS / 7 * 6), cubes[i].translation_matrix);

    // Move cube up and down.
    float up_down_speed = M_PI;
    matrix_translate_y((sin(rotation * up_down_speed + i * M_PI) / 5) + PILLAR_HEIGHT / 2.175, cubes[i].translation_matrix);

    // Rotate cube around the center to the corresponding edge.
    matrix_rotate_y(deg_to_rad(360) / (float)number_of_sides * (float)i, cubes[i].translation_matrix);

    // Apply general rotation to cube.
    matrix_rotate_y(rotation, cubes[i].translation_matrix);
  }
  // Request redrawing of window content.
  glutPostRedisplay();
}


/******************************************************************
*
* initialize
*
* This function is called to initialize rendering elements, setup
* vertex buffer objects, and to setup the vertex and fragment shader
*
*******************************************************************/

void initialize_view_matrix() {
  matrix_identity(view_matrix);
  float camera_distance = -7.0;
  matrix_translation(0.0, -1, camera_distance, view_matrix);
}

void initialize() {
  // Set background color based on system time.
  time_t current_time = time(NULL);
  int hour = localtime(&current_time)->tm_hour;
  if (hour >= 20 || hour <= 6) {
    glClearColor(R(0), G(25), B(50), 0.0);
  } else {
    glClearColor(R(220), G(220), B(220), 0.0);
  }

  // Enable depth testing.
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Initialize view matrix.
  initialize_view_matrix();


  // Setup external object
  wolf.obj_data.vertex_shader_file = "shader/vertex_shader.vs";
  wolf.obj_data.fragment_shader_file = "shader/fragment_shader.fs";
  init_ext_obj(&wolf, "models/wolf.obj");
  matrix_identity(wolf.obj_data.translation_matrix);


  /* Setup vertex, color, and index buffer objects for ROOF*/
  cone(20, BASE_RADIUS , ROOF_HEIGHT, &(roof.vertices), &(roof.vertices_size), &(roof.indices), &(roof.indices_size));
  setup_data_buffers(&roof);
  roof.vertex_shader_file = "shader/vertex_shader.vs";
  roof.fragment_shader_file = "shader/fragment_shader.fs";
  setup_shader_program(&roof);
  matrix_identity(roof.translation_matrix);

  /* Setup vertex, color, and index buffer objects for CENTER PILLAR*/
  cylinder(15, CENTER_PILLAR_RADIUS, PILLAR_HEIGHT, &(center_pillar.vertices), &(center_pillar.vertices_size), &(center_pillar.indices), &(center_pillar.indices_size));
  setup_data_buffers(&center_pillar);
  center_pillar.vertex_shader_file = "shader/vertex_shader.vs";
  center_pillar.fragment_shader_file = "shader/fragment_shader.fs";
  setup_shader_program(&center_pillar);
  matrix_identity(center_pillar.translation_matrix);

   /* Setup vertex, color, and index buffer objects for BASE*/
  cylinder(20, BASE_RADIUS, BASE_HEIGHT, &(base.vertices), &(base.vertices_size), &(base.indices), &(base.indices_size));
  setup_data_buffers(&base);
  base.vertex_shader_file = "shader/vertex_shader.vs";
  base.fragment_shader_file = "shader/fragment_shader.fs";
  setup_shader_program(&base);
  matrix_identity(base.translation_matrix);

  for (int i = 0; i < number_of_sides; i++) {
    struct object_data cube_object;

    cube(0.15, &(cube_object.vertices), &(cube_object.vertices_size), &(cube_object.indices), &(cube_object.indices_size));
    /* Setup vertex, color, and index buffer objects for cubes*/
    setup_data_buffers(&cube_object);
    cube_object.vertex_shader_file = "shader/vertex_shader.vs";
    cube_object.fragment_shader_file = "shader/fragment_shader.fs";
    setup_shader_program(&cube_object);
    matrix_identity(cube_object.translation_matrix);
    cubes[i] = cube_object;
  }

  for (int i = 0; i < number_of_sides; i++) {
    struct object_data pillar;

    cylinder(7, .03, PILLAR_HEIGHT, &(pillar.vertices), &(pillar.vertices_size), &(pillar.indices), &(pillar.indices_size));
    /* Setup vertex, color, and index buffer objects */
    setup_data_buffers(&pillar);
    pillar.vertex_shader_file = "shader/vertex_shader.vs";
    pillar.fragment_shader_file = "shader/fragment_shader.fs";
    setup_shader_program(&pillar);
    matrix_identity(pillar.translation_matrix);
    pillars[i] = pillar;
  }
}


void resize_window(int width, int height) {
  // Initialize projection matrix.
  glViewport(0, 0, width, height);

  matrix_identity(projection_matrix);
  float fovy = deg_to_rad(45);
  float aspect = (float)width / (float)height;
  float near_plane = 1.0;
  float far_plane = 50.0;
  matrix_perspective(fovy, aspect, near_plane, far_plane, projection_matrix);
}



void keyboard_event(unsigned char key, int x, int y) {
  (void)x;
  (void)y;

  switch(key) {
    case 'a': // left
      camera_translation_x = 1;
      break;
    case 'd': // right
      camera_translation_x = -1;
      break;
    case 'r': // up
      camera_translation_y = -1;
      break;
    case 'f': // down
      camera_translation_y = 1;
      break;
    case 'w': // forward
      camera_translation_z = 1;
      break;
    case 's': // backward
      camera_translation_z = -1;
      break;
    case 'q': // rotate left
      camera_rotation_y = 1;
      break;
    case 'e': // rotate right
      camera_rotation_y = -1;
      break;
  }

  glutPostRedisplay();
}

void keyboard_event_up(unsigned char key, int x, int y) {
  (void)x;
  (void)y;

  switch(key) {
    case 'a': // left
    case 'd': // right
      camera_translation_x = 0.0;
      break;
    case 'r': // up
    case 'f': // down
      camera_translation_y = 0.0;
      break;
    case 'w': // forward
    case 's': // backward
      camera_translation_z = 0.0;
      break;
    case 'q': // rotate left
    case 'e': // rotate right
      camera_rotation_y = 0.0;
      break;
    case 0x7f: // delete key
      initialize_view_matrix();
      break;
  }

  glutPostRedisplay();
}


void mouse_event(int button, int state, int x, int y) {
  (void)x;
  (void)y;

  if (state == GLUT_DOWN) {
    switch(button) {
      case GLUT_LEFT_BUTTON:
        printf("Left mouse button pressed.\n");
        break;

      case GLUT_MIDDLE_BUTTON:
        printf("Middle mouse button pressed.\n");
        break;

      case GLUT_RIGHT_BUTTON:
        printf("Right mouse button pressed.\n");
        break;
    }
  }
}


/******************************************************************
*
* main
*
* Main function to setup GLUT, GLEW, and enter rendering loop
*
*******************************************************************/

int main(int argc, char** argv) {
  srand(time(NULL));

  /* Initialize GLUT; set double buffered window and RGBA color model */
  glutInit(&argc, argv);

  #ifdef GLUT_CORE_PROFILE
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  #endif

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH
    #ifdef GLUT_3_2_CORE_PROFILE
    | GLUT_3_2_CORE_PROFILE // macOS
    #endif
  );

  int window_width = 800;
  int window_height = 600;
  int window_position_x = (glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2;
  int window_position_y = (glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2;
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(window_position_x, window_position_y);
  glutCreateWindow("Carousel");

  #ifdef __GLEW_H__
  /* Initialize GL extension wrangler */
  glewExperimental = GL_TRUE;
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return EXIT_FAILURE;
  }
  #endif

  // Setup scene and rendering parameters.
  initialize();

  // Specify GLUT callback functions.
  glutIdleFunc(on_idle);
  glutReshapeFunc(resize_window);
  glutDisplayFunc(display);

  glutKeyboardFunc(keyboard_event);
  glutKeyboardUpFunc(keyboard_event_up);
  glutMouseFunc(mouse_event);

  // Enter GLUT event processing loop.
  glutMainLoop();

  return EXIT_SUCCESS;
}
