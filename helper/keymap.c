#include "keymap.h"

bool keymap_set_key(struct keymap *keymap, unsigned char key, bool value) {
  switch(key) {
    case '1':
      keymap->one = value;
      break;
      case '2':
        keymap->two = value;
        break;
      case '6':
        keymap->six = value;
        break;
      case '7':
        keymap->seven = value;
        break;
      case '8':
        keymap->eight = value;
        break;
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
    case 'i':
      keymap->i = value;
      break;
    case 'j':
      keymap->j = value;
      break;
    case 'k':
      keymap->k = value;
      break;
    case 'l':
      keymap->l = value;
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
