#ifndef __CAMERA_H__
#define __CAMERA_H__

#define INIT_CAMERA {0, 0, 0, 0, 0, 0}

struct camera {
  float tx, ty, tz;
  float rx, ry, rz;
};

void print_camera(struct camera camera);

#endif // __CAMERA_H__
