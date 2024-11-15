#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "view.h"
#include "model.h"
static void scaleUp( Board src, Frame dst )
{
	for (int srcRow = 0; srcRow < BOARD_HEIGHT; srcRow++)
	{
		for (int srcCol = 0; srcCol < BOARD_WIDTH; srcCol++) 
		{
			for (int cellRow = 0; cellRow < FRAME_CELL_HEIGHT; cellRow++)
			{
				for (int cellCol = 0; cellCol < FRAME_CELL_WIDTH; cellCol++)
				{
					int dstRow = srcRow * FRAME_CELL_HEIGHT + cellRow;
					int dstCol = srcCol * FRAME_CELL_WIDTH + cellCol;

					dst[dstRow][dstCol] = src[srcRow][srcCol];
				}
			}
		}
	}
}



View* createView( void )
{
	View* this = malloc( sizeof(View) );

	/* ncurses settings */
	initscr();
	refresh();
	cbreak();
	noecho();
	keypad(stdscr, TRUE); /* enable arrow keys */
	curs_set(0);
	timeout(0);
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);

	/* ncurses windows */
	// this->wNextShape = newwin(
	// 	VIEW_NEXTSHAPE_HEIGHT, VIEW_NEXTSHAPE_WIDTH, VIEW_NEXTSHAPE_Y, VIEW_NEXTSHAPE_X);



	/* Create Game Board window with box around it */
	this->wBoardBox = newwin(VIEW_BOARDBOX_HEIGHT, VIEW_BOARDBOX_WIDTH, VIEW_BOARDBOX_Y, VIEW_BOARDBOX_X);
	box(this->wBoardBox, 0, 0);
	wrefresh(this->wBoardBox);
	this->wBoard = derwin( this->wBoardBox, VIEW_BOARD_HEIGHT, VIEW_BOARD_WIDTH, VIEW_BOARD_MARGIN, VIEW_BOARD_MARGIN);
	wrefresh(this->wBoard);

	this->wScore = newwin(VIEW_SCORE_HEIGHT, VIEW_SCORE_WIDTH, VIEW_SCORE_Y, VIEW_SCORE_X);
	// this->wInstructions = newwin(VIEW_INSTRUCTIONS_HEIGHT, VIEW_INSTRUCTIONS_WIDTH, VIEW_INSTRUCTIONS_Y, VIEW_INSTRUCTIONS_X);
	return this;
}

void renderBoard( View* this, Board* board )
{
	WINDOW* wBoard = this->wBoard;
	wmove(wBoard, 0, 0);
	scaleUp( *board, this->frame );

	for (int row = 0; row < FRAME_HEIGHT; row++)
	{
		for (int col = 0; col < FRAME_WIDTH; col++)
		{
			char value = this->frame[row][col] + 1;
			wattron(wBoard, COLOR_PAIR(value));
			waddch(wBoard, ' ');
			wattroff(wBoard, COLOR_PAIR(value));
		}
	}
	wrefresh(wBoard);
}

void renderGameStats(View* this, int level, int lines, int score)
{
	WINDOW* wScore = this->wScore;
	wmove(wScore, 0, 0);
	wattrset(wScore, A_BOLD);
	wprintw(wScore, "Level: %d\n", level);
	wprintw(wScore, "Lines: %d\n", lines);
	wprintw(wScore, "Score: %d\n", score);
	wrefresh(wScore);
}

void renderInstructions(View* this, char* instructions)
{
	WINDOW* wInstructions = this->wInstructions;
	wclear(wInstructions);
	wattrset(wInstructions, A_BOLD);
	wprintw(wInstructions, "%s", instructions);
	wrefresh(wInstructions);
}

void destroyView(View* this)
{
	delwin(this->wNextShape);
	delwin(this->wBoard);
	delwin(this->wBoardBox);
	delwin(this->wScore);
	endwin();
	free(this);
}