#ifndef __HELPER_H__
#define __HELPER_H__

#define unless(...) if (!(__VA_ARGS__))

#define deg_to_rad(degrees) (degrees * M_PI / 180.0)
#define rad_to_deg(radians) (radians * 180.0 / M_PI)

#endif // __HELPER_H__
