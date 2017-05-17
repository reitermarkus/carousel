#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#include <stdbool.h>

struct keymap {
  bool
    w,
    a,
    s,
    d,
    q,
    e,
    r,
    f,
    h,
    j,
    k,
    space
  ;
};

void keymap_init(struct keymap *keymap);

bool keymap_set_key(struct keymap *keymap, unsigned char key, bool value);

#endif // __KEYMAP_H__
