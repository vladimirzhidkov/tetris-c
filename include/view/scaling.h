#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define SCALING_HEIGHT   1
#define SCALING_WIDTH    2

void scale_up( u_int8_t *src, u_int8_t *dst, int src_height, int src_width );

#endif // UTILS_H
