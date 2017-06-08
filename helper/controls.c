#include "controls.h"

#include <math.h>

#include "helper/shared_headers.h"
#include "helper/setup_data_buffers.h"
#include "helper/keymap.h"
#include "helper/macros.h"
#include "helper/matrix.h"
#include "helper/color.h"
#include "helper/light.h"

extern matrix view_matrix;
extern matrix camera_matrix;
extern matrix mouse_matrix;

extern float brightness_factor;
extern float ambient_factor;
extern float diffuse_factor;
extern float specular_factor;

int ambient_toggle = 1;
int diffuse_toggle = 1;
int specular_toggle = 1;

extern struct light lights[2];

int light_toggle[] = {1, 1};
struct object_data light_object[2];

const float camera_height = -2;
const float camera_distance = -15.0;
const float camera_speed = 0.1;

struct keymap keymap = KEYMAP_INIT;

bool automatic_camera = true;

float rotate_x = 0;
float rotate_y = 0;

int mouse_x = 0;
int mouse_y = 0;

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

void keyboard_event_down(unsigned char key, int x, int y) {
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

void handle_controls() {
  if (keymap.space) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  // Brightness
  if (!keymap.h && keymap.j) { ambient_factor = fmax(ambient_factor - brightness_factor, 0.0); }
  if (keymap.h && !keymap.j) { ambient_factor = fmin(ambient_factor + brightness_factor, 1.0); }

  // Light Toggle
  if (keymap.one) { light_toggle[0] = !light_toggle[0]; keymap.one = false; }
  if (keymap.two) { light_toggle[1] = !light_toggle[1]; keymap.two = false; }

  // Ambient/Diffuse/Specular Toggle
  if (keymap.six)   { ambient_toggle  = !ambient_toggle;  keymap.six = false; }
  if (keymap.seven) { diffuse_toggle  = !diffuse_toggle;  keymap.seven = false; }
  if (keymap.eight) { specular_toggle = !specular_toggle; keymap.eight = false; }

  // Hue
  if (keymap.k) { lights[0].color.h = fmod(lights[0].color.h + 1, 360); update_light_colors(); }
  if (keymap.l) { lights[1].color.h = fmod(lights[1].color.h + 1, 360); update_light_colors(); }

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
}

void update_light_colors() {
  for (size_t i = 0; i < sizeof(lights) / sizeof(*lights); i++) {
    struct rgb color;
    hsv_to_rgb(lights[i].color, &color);

    for (size_t j = 0; j < light_object[i].vertex_count; j++) {
      SET_VERTEX_COLOR(light_object[i].vertices[j], color.r, color.g, color.b, 1.0);
    }

    setup_data_buffers(&light_object[i]);
  }
}
