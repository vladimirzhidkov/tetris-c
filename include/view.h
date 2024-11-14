#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include "shared.h"

#define FRAME_CELL_HEIGHT   1
#define FRAME_CELL_WIDTH    2
#define FRAME_HEIGHT        BOARD_HEIGHT * FRAME_CELL_HEIGHT
#define FRAME_WIDTH         BOARD_WIDTH * FRAME_CELL_WIDTH
#define FRAME_LEN           FRAME_HEIGHT * FRAME_WIDTH 

#define VIEW_NEXTSHAPE_HEIGHT	TETROMINO_SIZE * FRAME_CELL_HEIGHT + 2
#define VIEW_NEXTSHAPE_WIDTH	TETROMINO_SIZE * FRAME_CELL_WIDTH + 2
#define VIEW_NEXTSHAPE_X	0
#define VIEW_NEXTSHAPE_Y	0

#define VIEW_BOARD_MARGIN	1
#define VIEW_BOARDBOX_HEIGHT	BOARD_HEIGHT * FRAME_CELL_HEIGHT + 2 * VIEW_BOARD_MARGIN
#define VIEW_BOARDBOX_WIDTH	BOARD_WIDTH * FRAME_CELL_WIDTH + 2 * VIEW_BOARD_MARGIN
#define VIEW_BOARDBOX_Y		0
#define VIEW_BOARDBOX_X		VIEW_NEXTSHAPE_WIDTH

#define VIEW_BOARD_HEIGHT	BOARD_HEIGHT * FRAME_CELL_HEIGHT
#define VIEW_BOARD_WIDTH	BOARD_WIDTH * FRAME_CELL_WIDTH

#define VIEW_SCORE_HEIGHT	VIEW_BOARDBOX_HEIGHT
#define VIEW_SCORE_WIDTH	20
#define VIEW_SCORE_Y		1
#define VIEW_SCORE_X		VIEW_BOARDBOX_X + VIEW_BOARDBOX_WIDTH


typedef struct View
{
	WINDOW * wBoardBox;
	WINDOW * wBoard;

	WINDOW * wScore;
	WINDOW * wNextShape;
	WINDOW * wInstructions;

	char frame[FRAME_HEIGHT][FRAME_WIDTH];
} View;

View* createView( void );
void renderBoard( View*, char* frame );
void renderScore( View*, int level, int lines, int score );
void renderInstructions( View*, char* frame );
void destroyView( View* );

#endif // VIEW_H
