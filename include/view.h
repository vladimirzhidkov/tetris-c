#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include "shared.h"

#define VIEW_BOARDCELL_HEIGHT 1
#define VIEW_BOARDCELL_WIDTH 1

#define VIEW_NEXTSHAPE_HEIGHT	TETROMINO_SIZE * VIEW_BOARDCELL_HEIGHT + 2
#define VIEW_NEXTSHAPE_WIDTH	TETROMINO_SIZE * VIEW_BOARDCELL_WIDTH + 2
#define VIEW_NEXTSHAPE_X	0
#define VIEW_NEXTSHAPE_Y	0

#define VIEW_BOARD_MARGIN	1
#define VIEW_BOARDBOX_HEIGHT	BOARD_HEIGHT * VIEW_BOARDCELL_HEIGHT + 2 * VIEW_BOARD_MARGIN
#define VIEW_BOARDBOX_WIDTH	BOARD_WIDTH * VIEW_BOARDCELL_WIDTH + 2 * VIEW_BOARD_MARGIN
#define VIEW_BOARDBOX_Y		0
#define VIEW_BOARDBOX_X		VIEW_NEXTSHAPE_WIDTH

#define VIEW_BOARD_HEIGHT	BOARD_HEIGHT * VIEW_BOARDCELL_HEIGHT
#define VIEW_BOARD_WIDTH	BOARD_WIDTH * VIEW_BOARDCELL_WIDTH

#define VIEW_SCORE_HEIGHT	VIEW_BOARDBOX_HEIGHT
#define VIEW_SCORE_WIDTH	20
#define VIEW_SCORE_Y		1
#define VIEW_SCORE_X		VIEW_BOARDBOX_X + VIEW_BOARDBOX_WIDTH


typedef struct View
{
	WINDOW * wScore;
	WINDOW * wNextShape;

	WINDOW * wBoardBox;
	WINDOW * wBoard;

	WINDOW* wInstructions;
} View;

View* createView( void );
void renderBoard( View*, char* frame );
void renderScore( View*, int level, int lines, int score );
void renderInstructions( View*, char* frame );
void destroyView( View* );

#endif // VIEW_H
