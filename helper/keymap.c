#include "keymap.h"

void keymap_init(struct keymap *keymap) {
  keymap->w     = false;
  keymap->a     = false;
  keymap->s     = false;
  keymap->d     = false;
  keymap->q     = false;
  keymap->e     = false;
  keymap->r     = false;
  keymap->f     = false;
  keymap->h		= false;
  keymap->j		= false;
  keymap->k		= false;
  keymap->space = false;
}

bool keymap_set_key(struct keymap *keymap, unsigned char key, bool value) {
  switch(key) {
    case 'a':
      keymap->a = value;
      break;
    case 'd':
      keymap->d = value;
      break;
    case 'r':
      keymap->r = value;
      break;
    case 'f':
      keymap->f = value;
      break;
    case 'w':
      keymap->w = value;
      break;
    case 's':
      keymap->s = value;
      break;
    case 'q':
      keymap->q = value;
      break;
    case 'e':
      keymap->e = value;
      break;
    case 'h':
      keymap->h = value;
      break;
    case 'j':
      keymap->j = value;
      break;
    case 'k':
      keymap->k = value;
      break;
    case ' ':
      keymap->space = value;
      break;
    default:
      return false;
      break;
  }

  return true;
}
