#ifndef WBOARD_H 
#define WBOARD_H

#include <ncurses.h>
#include "board.h"
#include "scaling.h"

#define FRAME_BOARD_HEIGHT    BOARD_HEIGHT * SCALING_HEIGHT
#define FRAME_BOARD_WIDTH     BOARD_WIDTH * SCALING_WIDTH
#define FRAME_BOARD_LEN       FRAME_BOARD_HEIGHT * FRAME_BOARD_WIDTH 

#define WBOARD_HEIGHT	BOARD_HEIGHT * SCALING_HEIGHT + 2
#define WBOARD_WIDTH	BOARD_WIDTH * SCALING_WIDTH + 2
#define WBOARD_MARGIN	1

WINDOW * wboard_new( int y, int x );
void wboard_free( WINDOW * );

void wboard_update( WINDOW *, char *board );
#endif // WBOARD_H
