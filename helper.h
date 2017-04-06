#ifndef __HELPER_H__
#define __HELPER_H__

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#define unless(...) if (!(__VA_ARGS__))

#define deg_to_rad(degrees) (degrees * M_PI / 180.0)
#define rad_to_deg(radians) (radians * 180.0 / M_PI)

#define R(r) ((r) / 255.0)
#define G(g) ((g) / 255.0)
#define B(b) ((b) / 255.0)

#define RGB_RAND (rand() % 256)

#endif // __HELPER_H__
