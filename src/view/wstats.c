#include <ncurses.h>
#include "wstats.h"


WINDOW * wstats_new( int y, int x )
{
	return newwin(WSTATS_HEIGHT, WSTATS_WIDTH, y, x);
}

void wstats_free( WINDOW * w )
{
	delwin(w);
}

void wstats_update( WINDOW * this, int level, int lines, int score )
{
	wmove(this, 0, 0);
	wattrset(this, A_BOLD);
	wprintw(this, "Level: %d\n", level);
	wprintw(this, "Lines: %d\n", lines);
	wprintw(this, "Score: %d\n", score);
	wrefresh(this);
}
