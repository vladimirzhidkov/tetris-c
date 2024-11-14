#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "view.h"


// static char * str_repeat_char(char c, int n)
// {
// 	char* str = (char*)malloc((n + 1) * sizeof(char));
// 	memset(str, c, n);
// 	str[n] = 0;
// 	return str;
// }



static void scaleUp( char* src, char dst[][FRAME_WIDTH] )
{
	char (*s)[BOARD_WIDTH] = (char (*)[BOARD_WIDTH])src;

	for (int srcRow = 0; srcRow < BOARD_HEIGHT; srcRow++)
	{
		for (int srcCol = 0; srcCol < BOARD_WIDTH; srcCol++) 
		{
			for (int y = 0; y < FRAME_CELL_HEIGHT; y++)
			{
				for (int x = 0; x < FRAME_CELL_WIDTH; x++)
				{
					int dstRow = srcRow * FRAME_CELL_HEIGHT + y;
					int dstCol = srcCol * FRAME_CELL_WIDTH + x;

					dst[dstRow][dstCol] = s[srcRow][srcCol];
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
	// keypad(stdscr, TRUE);
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

void renderBoard(View* this, char* frame)
{
	scaleUp(frame, this->frame );

	// Display
	WINDOW* wBoard = this->wBoard;
	wmove(wBoard, 0, 0);
	for (int i = 0; i < FRAME_LEN; ++i)
	{
		char value = ((char *)this->frame)[i] + 1;
		wattron(wBoard, COLOR_PAIR(value));
		waddch(wBoard, ' ');
		// wprintw(wBoard, "%d", value);
		wattroff(wBoard, COLOR_PAIR(value));
	}
	// waddstr(this->wBoard, buffer);
	wrefresh(wBoard);
}

void renderScore(View* this, int level, int lines, int score)
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