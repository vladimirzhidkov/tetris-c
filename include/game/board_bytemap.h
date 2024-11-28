#ifndef BOARD_BYTETMAP_H
#define BOARD_BYTETMAP_H

#include <stdint.h>

#define BOARD_HEIGHT        15
#define BOARD_WIDTH         10
#define BOARD_LEN           BOARD_HEIGHT * BOARD_WIDTH 

typedef u_int8_t board_bytemap_t[BOARD_HEIGHT][BOARD_WIDTH];

#endif // BOARD_BYTETMAP_H
