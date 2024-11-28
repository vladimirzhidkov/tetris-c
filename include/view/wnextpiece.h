#ifndef WNEXTPIECE_H
#define WNEXTPIECE_H

#include <ncurses.h>
#include "scaling.h"
#include "tetromino_bytemap.h"

#define FRAME_NEXTPIECE_HEIGHT	TETROMINO_SIZE * SCALING_HEIGHT
#define FRAME_NEXTPIECE_WIDTH	TETROMINO_SIZE * SCALING_WIDTH
#define FRAME_NEXTPIECE_LEN		FRAME_NEXTPIECE_HEIGHT * FRAME_NEXTPIECE_WIDTH

#define WNEXTPIECE_HEIGHT	FRAME_NEXTPIECE_HEIGHT + 1
#define WNEXTPIECE_WIDTH	FRAME_NEXTPIECE_WIDTH

WINDOW * wnextpiece_new( int y, int x );
void wnextpiece_free( WINDOW * );

void wnextpiece_update( WINDOW *, char* piece );

#endif // WNEXTPIECE_H
