#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdint.h>
#include "view.h"
#include "wboard.h"
#include "wnextpiece.h"
#include "wstats.h"
#include "winstructions.h"

struct view_t
{
	WINDOW * wBoard;
	WINDOW * wStats;
	WINDOW * wNextPiece;
	WINDOW * wInstructions;
};

view_t * view_new( void )
{
	view_t *this = malloc( sizeof(view_t) );

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

	this->wBoard = wboard_new(WBOARD_Y, WBOARD_X);
	this->wNextPiece = wnextpiece_new(WNEXTPIECE_Y, WNEXTPIECE_X);
	this->wStats = wstats_new( WSTATS_Y, WSTATS_X);
	this->wInstructions = winstructions_new( WINSTRUCTIONS_Y, WINSTRUCTIONS_X);
	
	return this;
}

void view_free(view_t *this)
{
	wboard_free(this->wBoard);
	wnextpiece_free(this->wNextPiece);
	wstats_free(this->wStats);
	winstructions_free(this->wInstructions);
	endwin();
	free(this);
}

void update_board(view_t *this, u_int8_t *board)
{
	wboard_update(this->wBoard, board);
}

void update_nextpiece(view_t *this, u_int8_t *piece)
{
	wnextpiece_update(this->wNextPiece, piece);
}

void update_stats(view_t* this, int level, int lines, int score)
{
	wstats_update(this->wStats, level, lines, score);
}
