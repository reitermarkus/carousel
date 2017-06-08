#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#include <stdbool.h>

#define KEYMAP_INIT { \
  .one   = false,     \
  .two   = false,     \
  .six   = false,     \
  .seven = false,     \
  .eight = false,     \
  .w     = false,     \
  .a     = false,     \
  .s     = false,     \
  .d     = false,     \
  .q     = false,     \
  .e     = false,     \
  .r     = false,     \
  .f     = false,     \
  .h      = false,    \
  .j      = false,    \
  .k      = false,    \
  .l      = false,    \
  .space = false,     \
}

struct keymap {
  bool
    one,
    two,
    six,
    seven,
    eight,
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
    l,
    space
  ;
};

bool keymap_set_key(struct keymap *keymap, unsigned char key, bool value);

#endif // __KEYMAP_H__
