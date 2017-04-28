#include "camera.h"

#include <stdio.h>

void print_camera(struct camera camera) {
  printf("┌──────────┬──────────┬──────────┐\n");
  printf("| %8f | %8f | %8f |\n", camera.tx, camera.ty, camera.tz);
  printf("├──────────┼──────────┼──────────┤\n");
  printf("| %8f | %8f | %8f |\n", camera.rx, camera.ry, camera.rz);
  printf("└──────────┴──────────┴──────────┘\n");
}
