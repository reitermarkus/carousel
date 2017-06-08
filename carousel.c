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
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <time.h>

#include "helper/shared_headers.h"

/* Local includes */
#include "helper/shader_program.h"
#include "helper/controls.h"
#include "helper/draw.h"
#include "helper/keymap.h"
#include "helper/macros.h"
#include "helper/matrix.h"
#include "helper/init_external_obj.h"
#include "helper/light.h"
#include "helper/vertex.h"
#include "helper/color.h"
#include "helper/load_texture.h"
#include "parser/obj_parser.h"
#include "helper/setup_data_buffers.h"
#include "shape/cone.h"
#include "shape/cube.h"
#include "shape/cuboid.h"
#include "shape/cylinder.h"
#include "shape/flattened_cone.h"
#include "shape/hyper_rectangle.h"
#include "shape/polygon.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


matrix projection_matrix;
matrix view_matrix;
matrix camera_matrix;
matrix mouse_matrix;


struct light lights[] = {
  {
    .position = { 0, 1.55, 1.5 },
    .color    = { 90.0, 1.0, 1.0 },
  },
  {
    .position = { 0, 5.0, 7.0 },
    .color    = { 0.0, 1.0, 1.0 },
  },
};

float ambient_factor = 0.5;
float diffuse_factor = 0.5;
float specular_factor = 0.5;
float brightness_factor = 0.005;

extern int ambient_toggle;
extern int diffuse_toggle;
extern int specular_toggle;

extern int light_toggle[];
extern struct object_data light_object[];

enum { number_of_sides = 6 };
static struct object_data base;
static struct object_data center_pillar_bottom;
static struct object_data center_pillar_top;
static struct object_data center_pillar_mid_bottom;
static struct object_data center_pillar_mid_top;
static struct object_data roof;
static struct object_data pillars[number_of_sides];
static struct object_data planes[number_of_sides];
static struct object_data scene_floor;
static struct object_data palm_tree;

static const float PILLAR_HEIGHT = 2.5;
static const float BASE_HEIGHT = .25;
static const float BASE_RADIUS = 5;
static const float ROOF_HEIGHT = 1.5;
static const float CENTER_PILLAR_RADIUS = .8;
static const float LIGHT_SIZE = 0.2;


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

void display_object(struct object_data* object) {
  GLint model_matrix = glGetUniformLocation(object->shader_program, "model_matrix");
  if (model_matrix == -1) {
    fprintf(stderr, "Could not bind uniform model_matrix\n");
    exit(EXIT_FAILURE);
  }

  GLint view_uniform = glGetUniformLocation(object->shader_program, "view_matrix");
  if (view_uniform == -1) {
    fprintf(stderr, "Could not bind uniform ViewMatrix\n");
    exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(view_uniform, 1, GL_TRUE, *view_matrix);

  GLint light_count_uniform = glGetUniformLocation(object->shader_program, "light_count");
  if (view_uniform == -1) {
    fprintf(stderr, "Could not bind uniform lights[0].position\n");
    exit(EXIT_FAILURE);
  }
  glUniform1i(light_count_uniform, sizeof(lights) / sizeof(*lights));

  for (size_t i = 0; i < sizeof(lights) / sizeof(*lights); i++) {
    struct rgb color;
    hsv_to_rgb(lights[i].color, &color);

    char* pos_format_string = "lights[%d].position";
    int pos_string_size = snprintf(NULL, 0, pos_format_string, i);
    char * pos_string = alloca(pos_string_size + 1);
    sprintf(pos_string, pos_format_string, i);

    GLint light_pos_uniform = glGetUniformLocation(object->shader_program, pos_string);
    if (view_uniform == -1) {
      fprintf(stderr, "Could not bind uniform %s\n", pos_string);
      exit(EXIT_FAILURE);
    }
    glUniform3fv(light_pos_uniform, 1, &(lights[i].position.x));

    char* color_format_string = "lights[%d].color";
    int color_string_size = snprintf(NULL, 0, color_format_string, i);
    char * color_string = alloca(color_string_size + 1);
    sprintf(color_string, color_format_string, i);

    GLint light_col_uniform = glGetUniformLocation(object->shader_program, color_string);
    if (view_uniform == -1) {
      fprintf(stderr, "Could not bind uniform %s\n", color_string);
      exit(EXIT_FAILURE);
    }

    glUniform3f(light_col_uniform,
      color.r * light_toggle[i],
      color.g * light_toggle[i],
      color.b * light_toggle[i]
    );
  }

  GLint ambient_factor_uniform = glGetUniformLocation(object->shader_program,
      "ambient_factor");
  if (view_uniform == -1) {
    fprintf(stderr, "Could not bind uniform ambient_factor\n");
    exit(EXIT_FAILURE);
  }
  glUniform1f(ambient_factor_uniform, ambient_factor * ambient_toggle);

  GLint diffuse_factor_uniform = glGetUniformLocation(object->shader_program,
      "diffuse_factor");
  if (view_uniform == -1) {
    fprintf(stderr, "Could not bind uniform diffuse_factor\n");
    exit(EXIT_FAILURE);
  }
  glUniform1f(diffuse_factor_uniform, diffuse_factor * diffuse_toggle);

  GLint specular_factor_uniform = glGetUniformLocation(object->shader_program,
      "specular_factor");
  if (view_uniform == -1) {
    fprintf(stderr, "Could not bind uniform specular_factor\n");
    exit(EXIT_FAILURE);
  }
  glUniform1f(specular_factor_uniform, specular_factor * specular_toggle);

  draw(object, projection_matrix, view_matrix);
}

void display() {
  // Clear window with color specified in `initialize`.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  handle_controls();

  display_object(&base);
  display_object(&center_pillar_bottom);
  display_object(&center_pillar_top);
  display_object(&center_pillar_mid_bottom);
  display_object(&center_pillar_mid_top);
  display_object(&scene_floor);

  display_object(&palm_tree);

  for (int i = 0; i < number_of_sides; i++){
    display_object(&planes[i]);
  }

  for (size_t i = 0; i < sizeof(lights) / sizeof(*lights); i++) {
    display_object(&light_object[i]);
  }

  for (int i = 0; i < number_of_sides; i++) {
    display_object(&(pillars[i]));
  }

  display_object(&roof);

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

  matrix temp;
  matrix_identity(temp);

  float light_pos[3] = { 0, PILLAR_HEIGHT + BASE_HEIGHT - LIGHT_SIZE / 2, 1.5 };

  matrix_rotate_y(rotation, temp);
  matrix_multiply_pos(temp, light_pos);

  lights[0].position.x = light_pos[0];
  lights[0].position.y = light_pos[1];
  lights[0].position.z = light_pos[2];

  matrix_identity(light_object[0].translation_matrix);
  matrix_rotate_y(rotation, light_object[0].translation_matrix);
  matrix_translate_x(lights[0].position.x, light_object[0].translation_matrix);
  matrix_translate_y(lights[0].position.y, light_object[0].translation_matrix);
  matrix_translate_z(lights[0].position.z, light_object[0].translation_matrix);

  // Initialize center pillar matrix.
  matrix_identity(center_pillar_bottom.translation_matrix);

  // Move center pillar onto the base.
  matrix_translate_y(BASE_HEIGHT, center_pillar_bottom.translation_matrix);

  // Rotate center pillar.
  matrix_rotate_y(rotation, center_pillar_bottom.translation_matrix);
  matrix_multiply(mouse_matrix, center_pillar_bottom.translation_matrix, center_pillar_bottom.translation_matrix);

  // Initialize center pillar matrix.
  matrix_identity(center_pillar_top.translation_matrix);

  // Move center pillar onto the top.
  matrix_translate_y((PILLAR_HEIGHT + BASE_HEIGHT) * 0.63, center_pillar_top.translation_matrix);

  // Rotate center pillar.
  matrix_rotate_y(rotation, center_pillar_top.translation_matrix);
  matrix_multiply(mouse_matrix, center_pillar_top.translation_matrix, center_pillar_top.translation_matrix);

  // Initialize center pillar mid matrix.
  matrix_identity(center_pillar_mid_bottom.translation_matrix);

  // Move center pillar onto the top.
  matrix_translate_y((PILLAR_HEIGHT + BASE_HEIGHT) * 0.54, center_pillar_mid_bottom.translation_matrix);

  // Rotate center pillar.
  matrix_rotate_y(rotation, center_pillar_mid_bottom.translation_matrix);
  matrix_multiply(mouse_matrix, center_pillar_mid_bottom.translation_matrix, center_pillar_mid_bottom.translation_matrix);

  // Initialize center pillar mid matrix.
  matrix_identity(center_pillar_mid_top.translation_matrix);

  // Move center pillar onto the top.
  matrix_translate_y((PILLAR_HEIGHT + BASE_HEIGHT) * 0.45, center_pillar_mid_top.translation_matrix);

  // Rotate center pillar.
  matrix_rotate_y(rotation, center_pillar_mid_top.translation_matrix);
  matrix_multiply(mouse_matrix, center_pillar_mid_top.translation_matrix, center_pillar_mid_top.translation_matrix);

  // Initialize base matrix.
  matrix_identity(base.translation_matrix);

  // Rotate base.
  matrix_rotate_y(rotation, base.translation_matrix);
  matrix_multiply(mouse_matrix, base.translation_matrix, base.translation_matrix);

  // Initialize roof matrix.
  matrix_identity(roof.translation_matrix);

  // Rotate roof.
  matrix_rotate_y(rotation, roof.translation_matrix);

  // Move roof up above the pillars.
  matrix_translate_y(PILLAR_HEIGHT + BASE_HEIGHT, roof.translation_matrix);
  matrix_multiply(mouse_matrix, roof.translation_matrix, roof.translation_matrix);

  // Initialize floor matrix.
  matrix_identity(scene_floor.translation_matrix);

  // Move floor under base.
  matrix_translate_y(-BASE_HEIGHT + 0.07, scene_floor.translation_matrix);
  matrix_multiply(mouse_matrix, scene_floor.translation_matrix, scene_floor.translation_matrix);

  for (int i = 0; i < number_of_sides; i++) {
    // Initialize pillar matrix.
    matrix_identity(pillars[i].translation_matrix);

    // Move pillar onto the base.
    matrix_translate_y(BASE_HEIGHT, pillars[i].translation_matrix);

    // Move pillar towards the edge.
    matrix_translate_z(-(BASE_RADIUS / 3 * 2), pillars[i].translation_matrix);

    // Rotate pillar around the center to the corresponding edge.
    matrix_rotate_y(deg_to_rad(360) / (float)number_of_sides * (float)i, pillars[i].translation_matrix);

    // Apply general rotation to pillar.
    matrix_rotate_y(rotation, pillars[i].translation_matrix);
    matrix_multiply(mouse_matrix, pillars[i].translation_matrix, pillars[i].translation_matrix);
  }

  for (int i = 0; i < number_of_sides; i++){
    matrix_identity(planes[i].translation_matrix);
    matrix_rotate_y(deg_to_rad(130), planes[i].translation_matrix);
    matrix_translate_y(0.75, planes[i].translation_matrix);
    matrix_translate_x(-0.2, planes[i].translation_matrix);

    // Move cube up and down.
    float up_down_speed = M_PI;
    matrix_translate_y((sin(rotation * up_down_speed + i * M_PI) / 3) + PILLAR_HEIGHT / 4.175, planes[i].translation_matrix);

    // Move cube towards the edge.
    matrix_translate_z(-(BASE_RADIUS / 3 * 2), planes[i].translation_matrix);

    //~ matrix_translate_y(2.25, planes[i].translation_matrix);
    //~ matrix_translate_z(0.275, planes[i].translation_matrix);
    matrix_scale(0.6, planes[i].translation_matrix);

    // Rotate cube around the center to the corresponding edge.
    matrix_rotate_y(deg_to_rad(360) / (float)number_of_sides * (float)i, planes[i].translation_matrix);

    matrix_rotate_y(rotation, planes[i].translation_matrix);
    matrix_multiply(mouse_matrix, planes[i].translation_matrix, planes[i].translation_matrix);
  }

  // Request redrawing of window content.
  glutPostRedisplay();

  // Reduce GPU usage.
  struct timespec sleep_time = { 0, 10000000L };
  nanosleep(&sleep_time, NULL);
}


/******************************************************************
*
* initialize
*
* This function is called to initialize rendering elements, setup
* vertex buffer objects, and to setup the vertex and fragment shader
*
*******************************************************************/


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

  // Initialize matrices.
  matrix_identity(view_matrix);
  matrix_identity(camera_matrix);
  matrix_identity(mouse_matrix);

  GLuint shader_program = create_shader_program("shader/vertex_shader.vs", "shader/fragment_shader.fs");
  GLuint light_shader_program = create_shader_program("shader/vertex_shader.vs", "shader/light_shader.fs");

  GLuint plane_texture = load_texture("models/plane.jpg");
  GLuint grass_texture = load_texture("models/grass.png");
  GLuint palm_texture = load_texture("models/Hyophorbe_lagenicaulis_dif.jpg");

  init_object_data(&palm_tree);
  init_external_obj(&palm_tree, "models/Hyophorbe_lagenicaulis.obj");
  setup_data_buffers(&palm_tree);
  palm_tree.texture = palm_texture;
  palm_tree.shader_program = shader_program;
  matrix_scale(0.025, palm_tree.translation_matrix);
  matrix_translate_x(BASE_RADIUS * 2 * 0.7, palm_tree.translation_matrix);
  matrix_translate_z(-BASE_RADIUS * 2 * 0.7, palm_tree.translation_matrix);

  // External Object
  for (int i = 0; i < number_of_sides; i++) {
    init_object_data(&planes[i]);

    if (i == 0) {
      init_external_obj(&planes[i], "models/plane.obj");
      setup_data_buffers(&planes[i]);
    } else {
      memcpy(&planes[i], &planes[0], sizeof(planes[0]));
    }

    planes[i].texture = plane_texture;
    planes[i].shader_program = shader_program;
  }

  // Roof
  init_object_data(&roof);
  cone(20, BASE_RADIUS , ROOF_HEIGHT, &roof);
  setup_data_buffers(&roof);
  roof.shader_program = shader_program;

  // Center Pillar Bottom
  init_object_data(&center_pillar_bottom);
  flattened_cone(15, CENTER_PILLAR_RADIUS, CENTER_PILLAR_RADIUS * 0.66, PILLAR_HEIGHT * 0.40, &center_pillar_bottom);
  setup_data_buffers(&center_pillar_bottom);
  center_pillar_bottom.shader_program = shader_program;

  // Center Pillar Top
  init_object_data(&center_pillar_top);
  flattened_cone(15, CENTER_PILLAR_RADIUS * 0.66, CENTER_PILLAR_RADIUS, PILLAR_HEIGHT * 0.40, &center_pillar_top);
  setup_data_buffers(&center_pillar_top);
  center_pillar_top.shader_program = shader_program;

  // Center Pillar Mid Bottom
  init_object_data(&center_pillar_mid_bottom);
  flattened_cone(15, CENTER_PILLAR_RADIUS * 0.9, CENTER_PILLAR_RADIUS * 0.66, PILLAR_HEIGHT * 0.1, &center_pillar_mid_bottom);
  setup_data_buffers(&center_pillar_mid_bottom);
  center_pillar_mid_bottom.shader_program = shader_program;

  // Center Pillar Mid Top
  init_object_data(&center_pillar_mid_top);
  flattened_cone(15, CENTER_PILLAR_RADIUS * 0.66, CENTER_PILLAR_RADIUS * 0.9, PILLAR_HEIGHT * 0.1, &center_pillar_mid_top);
  setup_data_buffers(&center_pillar_mid_top);
  center_pillar_mid_top.shader_program = shader_program;

   // Base
  init_object_data(&base);
  cylinder(20, BASE_RADIUS, BASE_HEIGHT, &base);
  setup_data_buffers(&base);
  base.shader_program = shader_program;

  // Floor
  init_object_data(&scene_floor);
  hyper_rectangle(BASE_RADIUS * 4, BASE_HEIGHT, BASE_RADIUS * 4, &scene_floor);

  scene_floor.texture_count = scene_floor.index_count * 3;
  scene_floor.textures = calloc(scene_floor.texture_count, sizeof(*scene_floor.textures));

  // Set grass texture for top side of scene floor.
  scene_floor.textures[0 + 4 * 3].u = 0;
  scene_floor.textures[0 + 4 * 3].v = 0;
  scene_floor.textures[1 + 4 * 3].u = 0;
  scene_floor.textures[1 + 4 * 3].v = 2 * 1.77;
  scene_floor.textures[2 + 4 * 3].u = 2 * 1;
  scene_floor.textures[2 + 4 * 3].v = 2 * 1.77;
  scene_floor.textures[3 + 4 * 3].u = 2 * 1;
  scene_floor.textures[3 + 4 * 3].v = 0;

  setup_data_buffers(&scene_floor);

  scene_floor.texture = grass_texture;
  scene_floor.shader_program = shader_program;

  for (size_t i = 0; i < sizeof(lights) / sizeof(*lights); i++) {
    init_object_data(&light_object[i]);
    cube(LIGHT_SIZE, &light_object[i]);

    for (size_t j = 0; j < light_object[i].vertex_count; j++) {
      struct rgb color;
      hsv_to_rgb(lights[i].color, &color);

      SET_VERTEX_COLOR(light_object[i].vertices[j], color.r, color.g, color.b, 1.0);
    }

    setup_data_buffers(&light_object[i]);
    light_object[i].shader_program = light_shader_program;
    matrix_translate_x(lights[i].position.x, light_object[i].translation_matrix);
    matrix_translate_y(lights[i].position.y, light_object[i].translation_matrix);
    matrix_translate_z(lights[i].position.z * 1.1, light_object[i].translation_matrix);
  }

  // Pillars
  for (int i = 0; i < number_of_sides; i++) {
    init_object_data(&pillars[i]);
    cylinder(7, .03, PILLAR_HEIGHT, &pillars[i]);
    setup_data_buffers(&pillars[i]);
    pillars[i].shader_program = shader_program;
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

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // Setup scene and rendering parameters.
  initialize();

  // Specify GLUT callback functions.
  glutIdleFunc(on_idle);
  glutReshapeFunc(resize_window);
  glutDisplayFunc(display);

  // Enable blending for transparency.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glutKeyboardFunc(keyboard_event_down);
  glutKeyboardUpFunc(keyboard_event_up);

  glutMotionFunc(mouse_motion);
  glutPassiveMotionFunc(mouse_passive);

  // Enter GLUT event processing loop.
  glutMainLoop();

  return EXIT_SUCCESS;
}
