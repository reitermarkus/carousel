#ifndef __CONTROLS_H__
#define __CONTROLS_H__

#include "helper/keymap.h"

void mouse_passive(int x, int y);
void mouse_motion(int x, int y);

void keyboard_event_down(unsigned char key, int x, int y);
void keyboard_event_up(unsigned char key, int x, int y);

void handle_controls();
void update_light_colors();

#endif // __CONTROLS_H__
