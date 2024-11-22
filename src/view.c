#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "view.h"

#define scaleBoardUp(src, dst) \
	scale_up( src, dst, BOARD_HEIGHT, BOARD_WIDTH, CELL_HEIGHT, CELL_WIDTH )

#define scalePieceUp(src, dst) \
	scale_up( src, dst, TETROMINO_SIZE, TETROMINO_SIZE, CELL_HEIGHT, CELL_WIDTH )



void scale_up( char* src, char* dst, int src_height, int src_width, int height_scale, int width_scale )
{
	int dst_width = src_width * width_scale;
	char (*s)[src_width] = (char (*)[src_width])src;
	char (*d)[dst_width] = (char (*)[dst_width])dst;

	for (int src_row = 0; src_row < src_height; src_row++)
	{
		for (int src_col = 0; src_col < src_width; src_col++) 
		{
			for (int cell_row = 0; cell_row < height_scale; cell_row++)
			{
				for (int cell_col = 0; cell_col < width_scale; cell_col++)
				{
					int dst_row = src_row * height_scale + cell_row;
					int dst_col = src_col * width_scale + cell_col;

					d[dst_row][dst_col] = s[src_row][src_col];
				}
			}
		}
	}
}


View* view_new( void )
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

	/* Next Piece window */
	this->wNextPiece = newwin(
	 	W_NEXTPIECE_HEIGHT, W_NEXTPIECE_WIDTH, W_NEXTPIECE_Y, W_NEXTPIECE_X);

	/* Game Board window with box around it */
	this->wBoardBox = newwin(W_BOARD_HEIGHT, W_BOARD_WIDTH, W_BOARD_Y, W_BOARD_X);
	box(this->wBoardBox, 0, 0);
	wrefresh(this->wBoardBox);
	this->wBoard = derwin( this->wBoardBox, F_BOARD_HEIGHT, F_BOARD_WIDTH, W_BOARD_MARGIN, W_BOARD_MARGIN);
	wrefresh(this->wBoard);

	/* Stats window */
	this->wStats = newwin(W_STATS_HEIGHT, W_STATS_WIDTH, W_STATS_Y, W_STATS_X);

	/* Instructions window */
	this->wInstructions = newwin(
		W_INSTRUCTIONS_HEIGHT, W_INSTRUCTIONS_WIDTH, W_INSTRUCTIONS_Y, W_INSTRUCTIONS_X);

	return this;
}

void renderBoard( View* this, char* board )
{
	scaleBoardUp( board, this->boardFrame );
	wmove(this->wBoard, 0, 0);

	for (int i = 0; i < F_BOARD_LEN; ++i)
	{
		char value = this->boardFrame[i] + 1;
		wattron(this->wBoard, COLOR_PAIR(value));
		waddch(this->wBoard, ' ');
		wattroff(this->wBoard, COLOR_PAIR(value));
	}
	wrefresh(this->wBoard);
}

void renderNextPiece(View* this, char* piece)
{
	scalePieceUp( (char *)piece, this->nextPieceFrame );
	wmove(this->wNextPiece, 0, 0);
	wattrset(this->wNextPiece, A_BOLD);
	waddstr(this->wNextPiece, "Next Piece:\n\n");

	for (int i = 0; i < F_NEXTPIECE_LEN; ++i)
	{
		char value = this->nextPieceFrame[i] + 1;
		wattron(this->wNextPiece, COLOR_PAIR(value));
		waddch(this->wNextPiece, ' ');
		wattroff(this->wNextPiece, COLOR_PAIR(value));
	}
	wrefresh(this->wNextPiece);
}

void renderStats(View* this, int level, int lines, int score)
{
	WINDOW* wStats = this->wStats;
	wmove(wStats, 0, 0);
	wattrset(wStats, A_BOLD);
	wprintw(wStats, "Level: %d\n", level);
	wprintw(wStats, "Lines: %d\n", lines);
	wprintw(wStats, "Score: %d\n", score);
	wrefresh(wStats);
}

void renderInstructions(View* this, char* instructions)
{
	WINDOW* wInstructions = this->wInstructions;
	wmove(wInstructions, 0, 0);
	wattrset(wInstructions, A_BOLD);
	wprintw(wInstructions, "%s", instructions);
	wrefresh(wInstructions);
}

void view_free(View* this)
{
	delwin(this->wNextPiece);
	delwin(this->wBoard);
	delwin(this->wBoardBox);
	delwin(this->wStats);
	delwin(this->wInstructions);
	endwin();
	free(this);
}