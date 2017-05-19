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
#include <time.h>

#include "helper/shared_headers.h"

/* Local includes */
#include "helper/shader_program.h"

#include "helper/draw.h"
#include "helper/keymap.h"
#include "helper/macros.h"
#include "helper/matrix.h"
#include "helper/light.h"
#include "helper/vertex.h"
#include "helper/color.h"
#include "helper/load_texture.h"
#include "parser/obj_parser.h"
#include "shape/cone.h"
#include "shape/cube.h"
#include "shape/cuboid.h"
#include "shape/cylinder.h"
#include "shape/flattened_cone.h"
#include "shape/hyper_rectangle.h"
#include "shape/polygon.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*----------------------------------------------------------------*/

static matrix projection_matrix;
static matrix view_matrix;
static matrix camera_matrix;
static matrix mouse_matrix;

/**/

struct light lights[] = {
  {
    .position = { 0, 1.55, 1.5 },
    .color    = { 90.0, 1.0, 1.0 },
  },
  {
    .position = { 0, 4.0, 7.0 },
    .color    = { 0.0, 1.0, 1.0 },
  },
};

float ambient_factor = 0.5;
float diffuse_factor = 0.5;
float specular_factor = 0.5;

float factor_brightness = 0.005;
float factor_hue = 0.05;

int ambient_toggle = 1;
int diffuse_toggle = 1;
int specular_toggle = 1;

int light_toggle[2] = {1, 1};

/**/

static const float camera_height = -2;
static const float camera_distance = -15.0;
static const float camera_speed = 0.1;

enum { number_of_sides = 8 };
static struct object_data base;
static struct object_data center_pillar_bottom;
static struct object_data center_pillar_top;
static struct object_data center_pillar_mid_bottom;
static struct object_data center_pillar_mid_top;
static struct object_data roof;
static struct object_data pillars[number_of_sides];
static struct object_data cubes[number_of_sides];
static struct object_data scene_floor;

static struct object_data light_object[2];

/* Structures for loading of OBJ data */
GLuint tiger_texture;
struct object_data extern_object;


/*----------------------------------------------------------------------*/

struct graphic_buffer* gb_extern_object;

/*----------------------------------------------------------------------*/


static const float PILLAR_HEIGHT = 1.5;
static const float BASE_HEIGHT = .15;
static const float BASE_RADIUS = 2.5;
static const float ROOF_HEIGHT = 1.0;
static const float CENTER_PILLAR_RADIUS = 0.5;

struct keymap keymap;

bool automatic_camera = true;

float rotate_x = 0;
float rotate_y = 0;

int mouse_x;
int mouse_y;


void mouse_passive(int x, int y) {
  mouse_x = x;
  mouse_y = y;
}

void mouse_motion(int x, int y) {
  static const float SPEED = 0.1 / (2 * M_PI);

  rotate_x += (mouse_y - y) * SPEED; // rotate around x axis when mouse moves up/down
  rotate_y += (mouse_x - x) * SPEED; // rotate around y axis when mouse moves left/right

  mouse_passive(x, y);
  glutPostRedisplay();
}


/******************************************************************
*
* setup_data_buffers
*
* Create buffer objects and load data into buffers
*
*******************************************************************/

void setup_data_buffers(struct object_data* object) {
  calculate_normals(object);

  glGenVertexArrays(1, &(object->vao));
  glBindVertexArray(object->vao);

  // Bind buffer object for vertices and colors.
  glGenBuffers(1, &(object->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, object->vertex_count * sizeof(*object->vertices), object->vertices, GL_STATIC_DRAW);
  free(object->vertices);

  // Bind buffer object for indices.
  glGenBuffers(1, &(object->ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->index_count * sizeof(*object->indices), object->indices, GL_STATIC_DRAW);
  free(object->indices);

  // Bind vertex positions.
  glEnableVertexAttribArray(v_position);
  glVertexAttribPointer(
    v_position,            // attribute index
    3,                     // attribute length (x, y, z)
    GL_FLOAT,              // attribute type
    GL_FALSE,              // normalized?
    sizeof(struct vertex), // offset between indices
    0                      // offset to vertex values
  );

  // Bind vertex colors.
  glEnableVertexAttribArray(v_color);
  glVertexAttribPointer(
    v_color,                         // attribute index
    4,                               // attribute length (r, g, b, a)
    GL_FLOAT,                        // attribute type
    GL_FALSE,                        // normalized?
    sizeof(struct vertex),           // offset between indices
    (GLvoid*)sizeof(struct position) // offset to color values
  );

  glEnableVertexAttribArray(v_normal);
  glVertexAttribPointer(
    v_normal,                                                 // attribute index
    3,                                                        // attribute length (r, g, b, a)
    GL_FLOAT,                                                 // attribute type
    GL_FALSE,                                                 // normalized?
    sizeof(struct vertex),                                    // offset between indices
    (GLvoid*)(sizeof(struct position) + sizeof(struct color)) // offset to color values
  );

  if (object->texture_count != 0) {
    glGenBuffers(1, &(object->tbo));
    glBindBuffer(GL_ARRAY_BUFFER, object->tbo);
    glBufferData(GL_ARRAY_BUFFER, object->texture_count * sizeof(*object->textures), object->textures, GL_STATIC_DRAW);
    free(object->textures);

    // Bind vertex textures.
    glEnableVertexAttribArray(v_texture);
   	glVertexAttribPointer(
   		v_texture,                                                // attribute index
   		2,                                                        // attribute length (u, v)
   		GL_FLOAT,                                                 // attribute type
   		GL_FALSE,                                                 // normalized?
   		0,                                                        // offset between indices
   		0                                                         // offset to texture values
   	);
  }

  // Bind indices.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);

  glBindVertexArray(0);

  glDisableVertexAttribArray(v_position);
  glDisableVertexAttribArray(v_color);
  glDisableVertexAttribArray(v_normal);
  glDisableVertexAttribArray(v_texture);
}

/******************************************************************
*
* Setup and Initialization for external ".obj" files
*
*******************************************************************/

void init_ext_obj(struct object_data* obj, char* filename){
  obj_scene_data ext_obj;

  if (parse_obj_scene_data(&ext_obj, filename) == -1) {
    printf("Could not load file. Exiting.\n");
    exit(EXIT_FAILURE);
  }

  // Copy mesh data from structs into appropriate arrays.

  obj->vertex_count = ext_obj.vertex_count;
  obj->index_count = ext_obj.face_count;
  obj->texture_count = ext_obj.face_count * 3;

  obj->vertices = malloc(obj->vertex_count * sizeof(*obj->vertices));
  obj->indices = malloc(obj->index_count * sizeof(*obj->indices));
  obj->textures = malloc(obj->texture_count * sizeof(*obj->textures));

  // Vertices
  for (int i = 0; i < ext_obj.vertex_count; i++) {
    SET_VERTEX_POSITION(obj->vertices[i],
      ext_obj.vertex_list[i]->e[0],
      ext_obj.vertex_list[i]->e[1],
      ext_obj.vertex_list[i]->e[2]
    );

    SET_VERTEX_COLOR(obj->vertices[i], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  }

  for (int i = 0; i < ext_obj.face_count; i++) {
    // Indices
    obj->indices[i].a = ext_obj.face_list[i]->vertex_index[0];
    obj->indices[i].b = ext_obj.face_list[i]->vertex_index[1];
    obj->indices[i].c = ext_obj.face_list[i]->vertex_index[2];

    // Textures
    for (int j = 0; j < 3; j++) {
      int index = ext_obj.face_list[i]->texture_index[j];

      if (index >= 0) {
        obj->textures[i + j].u = ext_obj.vertex_texture_list[index]->e[0];
        obj->textures[i + j].v = ext_obj.vertex_texture_list[index]->e[1];
      }
    }
  }

  delete_obj_scene_data(&ext_obj);
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

void setup_shader_program(struct object_data* object, const char* vertex_shader_file, const char* fragment_shader_file) {
  // Put linked shader program into drawing pipeline.
  object->vertex_shader_file = vertex_shader_file;
  object->fragment_shader_file = fragment_shader_file;
  object->shader_program = create_shader_program(object->vertex_shader_file, object->fragment_shader_file);
}

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
    char * pos_string = malloc(pos_string_size + 1);
    sprintf(pos_string, pos_format_string, i);

  	GLint light_pos_uniform = glGetUniformLocation(object->shader_program, pos_string);
  	if (view_uniform == -1) {
  		fprintf(stderr, "Could not bind uniform %s\n", pos_string);
  		exit(EXIT_FAILURE);
  	}
  	glUniform3fv(light_pos_uniform, 1, &(lights[i].position.x));

    free(pos_string);

    char* color_format_string = "lights[%d].color";
    int color_string_size = snprintf(NULL, 0, color_format_string, i);
    char * color_string = malloc(color_string_size + 1);
    sprintf(color_string, color_format_string, i);

  	GLint light_col_uniform = glGetUniformLocation(object->shader_program, color_string);
  	if (view_uniform == -1) {
  		fprintf(stderr, "Could not bind uniform %s\n", color_string);
  		exit(EXIT_FAILURE);
  	}

    free(color_string);

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

  if (keymap.space) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  // Brightness
  if (!keymap.h && keymap.j) { ambient_factor = fmax(ambient_factor - factor_brightness, 0.0); }
  if (keymap.h && !keymap.j) { ambient_factor = fmin(ambient_factor + factor_brightness, 1.0); }

  // Light Toggle
  if (keymap.one) { light_toggle[0] = !light_toggle[0]; keymap.one = false; }
  if (keymap.two) { light_toggle[1] = !light_toggle[1]; keymap.two = false; }

  // Ambient/Diffuse/Specular Toggle
  if (keymap.six)   { ambient_toggle  = !ambient_toggle;  keymap.six = false; }
  if (keymap.seven) { diffuse_toggle  = !diffuse_toggle;  keymap.seven = false; }
  if (keymap.eight) { specular_toggle = !specular_toggle; keymap.eight = false; }

  // Hue
  if (keymap.k) { lights[0].color.h = fmod(lights[0].color.h + 1, 360); }
  if (keymap.l) { lights[1].color.h = fmod(lights[1].color.h + 1, 360); }

  if (!automatic_camera) {
    if (keymap.a && !keymap.d) { matrix_translate_x(+camera_speed, camera_matrix);         } // left
    if (!keymap.a && keymap.d) { matrix_translate_x(-camera_speed, camera_matrix);         } // right
    if (keymap.w && !keymap.s) { matrix_translate_z(+camera_speed, camera_matrix);         } // forward
    if (!keymap.w && keymap.s) { matrix_translate_z(-camera_speed, camera_matrix);         } // backward
    if (keymap.q && !keymap.e) { matrix_rotate_y(+camera_speed / 2 / M_PI, camera_matrix); } // rotate left
    if (!keymap.q && keymap.e) { matrix_rotate_y(-camera_speed / 2 / M_PI, camera_matrix); } // rotate right
    if (keymap.r && !keymap.f) { matrix_translate_y(-camera_speed / 2.0, camera_matrix);   } // up
    if (!keymap.r && keymap.f) { matrix_translate_y(+camera_speed / 2.0, camera_matrix);   } // down
  } else {
    long elapsed_time = glutGet(GLUT_ELAPSED_TIME); // ms
    float rotation = (elapsed_time / 500.0) * camera_speed;

    matrix_identity(camera_matrix);

    matrix_translate_x(camera_distance * sinf(rotation), camera_matrix);
    matrix_translate_y(camera_height, camera_matrix);
    matrix_translate_z(camera_distance * cosf(rotation), camera_matrix);
    matrix_rotate_y(-rotation, camera_matrix);
  }

  matrix_identity(view_matrix);
  matrix_multiply(camera_matrix, view_matrix, view_matrix);

  matrix_identity(mouse_matrix);
  matrix_rotate_y(-rotate_y, mouse_matrix);
  matrix_rotate_x(-rotate_x, mouse_matrix);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tiger_texture);
  glUniform1i(glGetUniformLocation(extern_object.shader_program, "tiger_texture"), 0);

  display_object(&extern_object);
  display_object(&base);
  display_object(&center_pillar_bottom);
  display_object(&center_pillar_top);
  display_object(&center_pillar_mid_bottom);
  display_object(&center_pillar_mid_top);
  display_object(&scene_floor);

  for (size_t i = 0; i < sizeof(lights) / sizeof(*lights); i++) {
    display_object(&light_object[i]);
  }

  for (int i = 0; i < number_of_sides; i++) {
    display_object(&(pillars[i]));
    display_object(&(cubes[i]));
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

  float light_pos[3] = { 0, 1.55, 1.5 };

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
    matrix_translate_z(-(BASE_RADIUS / 7 * 6), pillars[i].translation_matrix);

    // Rotate pillar around the center to the corresponding edge.
    matrix_rotate_y(deg_to_rad(360) / (float)number_of_sides * (float)i, pillars[i].translation_matrix);

    // Apply general rotation to pillar.
    matrix_rotate_y(rotation, pillars[i].translation_matrix);
    matrix_multiply(mouse_matrix, pillars[i].translation_matrix, pillars[i].translation_matrix);
  }

  matrix_identity(extern_object.translation_matrix);
  matrix_translate_y(2.25, extern_object.translation_matrix);
  matrix_translate_z(0.275, extern_object.translation_matrix);
  matrix_scale(6, extern_object.translation_matrix);
  matrix_rotate_y(rotation, extern_object.translation_matrix);
  matrix_multiply(mouse_matrix, extern_object.translation_matrix, extern_object.translation_matrix); 

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
    matrix_multiply(mouse_matrix, cubes[i].translation_matrix, cubes[i].translation_matrix);
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

  tiger_texture = load_texture("models/tigercub/tigercub.png");

  // External Object
  init_object_data(&extern_object);
  init_ext_obj(&extern_object, "models/tigercub.obj");
  setup_data_buffers(&extern_object);
  setup_shader_program(&extern_object, "shader/vertex_shader.vs", "shader/fragment_shader.fs");

  // Roof
  init_object_data(&roof);
  cone(20, BASE_RADIUS , ROOF_HEIGHT, &roof);
  setup_data_buffers(&roof);
  setup_shader_program(&roof, "shader/vertex_shader.vs", "shader/fragment_shader.fs");

  // Center Pillar Bottom
  init_object_data(&center_pillar_bottom);
  flattened_cone(15, CENTER_PILLAR_RADIUS, CENTER_PILLAR_RADIUS * 0.70, PILLAR_HEIGHT * 0.40, &center_pillar_bottom);
  setup_data_buffers(&center_pillar_bottom);
  setup_shader_program(&center_pillar_bottom, "shader/vertex_shader.vs", "shader/fragment_shader.fs");

  // Center Pillar Top
  init_object_data(&center_pillar_top);
  flattened_cone(15, CENTER_PILLAR_RADIUS  * 0.70, CENTER_PILLAR_RADIUS, PILLAR_HEIGHT * 0.40, &center_pillar_top);
  setup_data_buffers(&center_pillar_top);
  setup_shader_program(&center_pillar_top, "shader/vertex_shader.vs", "shader/fragment_shader.fs");

  // Center Pillar Mid Bottom
  init_object_data(&center_pillar_mid_bottom);
  flattened_cone(15, CENTER_PILLAR_RADIUS, CENTER_PILLAR_RADIUS * 0.70, PILLAR_HEIGHT * 0.1, &center_pillar_mid_bottom);
  setup_data_buffers(&center_pillar_mid_bottom);
  setup_shader_program(&center_pillar_mid_bottom, "shader/vertex_shader.vs", "shader/fragment_shader.fs");

  // Center Pillar Mid Top
  init_object_data(&center_pillar_mid_top);
  flattened_cone(15, CENTER_PILLAR_RADIUS  * 0.70, CENTER_PILLAR_RADIUS, PILLAR_HEIGHT * 0.1, &center_pillar_mid_top);
  setup_data_buffers(&center_pillar_mid_top);
  setup_shader_program(&center_pillar_mid_top, "shader/vertex_shader.vs", "shader/fragment_shader.fs");

   // Base
  init_object_data(&base);
  cylinder(20, BASE_RADIUS, BASE_HEIGHT, &base);
  setup_data_buffers(&base);
  setup_shader_program(&base, "shader/vertex_shader.vs", "shader/fragment_shader.fs");

  // Floor
  init_object_data(&scene_floor);
  hyper_rectangle(BASE_RADIUS * 4, BASE_HEIGHT, BASE_RADIUS * 4, &scene_floor);
  setup_data_buffers(&scene_floor);
  setup_shader_program(&scene_floor, "shader/vertex_shader.vs", "shader/fragment_shader.fs");

  for (size_t i = 0; i < sizeof(lights) / sizeof(*lights); i++) {
    init_object_data(&light_object[i]);
    cube(0.2, &light_object[i]);

    for (size_t j = 0; j < light_object[i].vertex_count; j++) {
      SET_VERTEX_COLOR(light_object[i].vertices[j], lights[i].color.h, lights[i].color.s, lights[i].color.v, 1.0);
    }

    setup_data_buffers(&light_object[i]);
    setup_shader_program(&light_object[i], "shader/vertex_shader.vs", "shader/fragment_shader.fs");
    matrix_translate_x(lights[i].position.x, light_object[i].translation_matrix);
    matrix_translate_y(lights[i].position.y, light_object[i].translation_matrix);
    matrix_translate_z(lights[i].position.z * 1.1, light_object[i].translation_matrix);
  }

  // Cubes
  for (int i = 0; i < number_of_sides; i++) {
    init_object_data(&cubes[i]);
    cube(0.3, &cubes[i]);
    setup_data_buffers(&cubes[i]);
    setup_shader_program(&cubes[i], "shader/vertex_shader.vs", "shader/fragment_shader.fs");
  }

  // Pillars
  for (int i = 0; i < number_of_sides; i++) {
    init_object_data(&pillars[i]);
    cylinder(7, .03, PILLAR_HEIGHT, &pillars[i]);
    setup_data_buffers(&pillars[i]);
    setup_shader_program(&pillars[i], "shader/vertex_shader.vs", "shader/fragment_shader.fs");
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

  keymap_set_key(&keymap, key, true);

  if (keymap.a || keymap.w || keymap.s || keymap.d || keymap.q || keymap.e || keymap.r || keymap.f) {
    automatic_camera = false;
  }

  glutPostRedisplay();
}

void keyboard_event_up(unsigned char key, int x, int y) {
  (void)x;
  (void)y;

  keymap_set_key(&keymap, key, false);

  switch(key) {
    case 0x7f: // delete key
      if (!automatic_camera) {
        automatic_camera = true;
      }
      rotate_x = 0;
      rotate_y = 0;
      break;
  }

  glutPostRedisplay();
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

  keymap_init(&keymap);
  glutKeyboardFunc(keyboard_event);
  glutKeyboardUpFunc(keyboard_event_up);

  glutMotionFunc(mouse_motion);
  glutPassiveMotionFunc(mouse_passive);

  // Enter GLUT event processing loop.
  glutMainLoop();

  return EXIT_SUCCESS;
}
