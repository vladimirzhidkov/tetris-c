#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include "model.h"

#define CELL_HEIGHT   2
#define CELL_WIDTH    4


/* frame dimensions */
#define F_BOARD_HEIGHT    BOARD_HEIGHT * CELL_HEIGHT
#define F_BOARD_WIDTH     BOARD_WIDTH * CELL_WIDTH
#define F_BOARD_LEN       F_BOARD_HEIGHT * F_BOARD_WIDTH 

#define F_NEXTPIECE_HEIGHT	TETROMINO_SIZE * CELL_HEIGHT
#define F_NEXTPIECE_WIDTH	TETROMINO_SIZE * CELL_WIDTH
#define F_NEXTPIECE_LEN		F_NEXTPIECE_HEIGHT * F_NEXTPIECE_WIDTH


/* ncurses window dimensions */

#define W_NEXTPIECE_HEIGHT	F_NEXTPIECE_HEIGHT + 2
#define W_NEXTPIECE_WIDTH	F_NEXTPIECE_WIDTH
#define W_NEXTPIECE_X		1
#define W_NEXTPIECE_Y		1

#define W_BOARD_HEIGHT	BOARD_HEIGHT * CELL_HEIGHT + 2
#define W_BOARD_WIDTH	BOARD_WIDTH * CELL_WIDTH + 2
#define W_BOARD_MARGIN	1
#define W_BOARD_Y		0
#define W_BOARD_X		W_NEXTPIECE_X + W_NEXTPIECE_WIDTH + 1

#define W_STATS_HEIGHT	W_BOARD_HEIGHT
#define W_STATS_WIDTH	20
#define W_STATS_Y		1
#define W_STATS_X		W_BOARD_X + W_BOARD_WIDTH

#define W_INSTRUCTIONS_HEIGHT	W_BOARD_HEIGHT - W_NEXTPIECE_HEIGHT
#define W_INSTRUCTIONS_WIDTH	W_NEXTPIECE_WIDTH
#define W_INSTRUCTIONS_Y		W_NEXTPIECE_Y + W_NEXTPIECE_HEIGHT
#define W_INSTRUCTIONS_X		W_NEXTPIECE_X


typedef struct View
{
	char boardFrame[F_BOARD_LEN];
	char nextPieceFrame[F_NEXTPIECE_LEN];

	WINDOW * wBoardBox;
	WINDOW * wBoard;
	WINDOW * wStats;
	WINDOW * wNextPiece;
	WINDOW * wInstructions;
} View;

View* createView( void );
void renderNextPiece( View*, char* piece );
void renderBoard( View*, char* board );
void renderStats( View*, int level, int lines, int score );
void renderInstructions( View*, char* frame );
void destroyView( View* );

#endif // VIEW_H
