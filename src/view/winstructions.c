#include <ncurses.h>
#include "winstructions.h"


WINDOW * winstructions_new( int y, int x )
{
	WINDOW* this = newwin(WINSTRUCTIONS_HEIGHT, WINSTRUCTIONS_WIDTH, y, x);
    wmove(this, 0, 0);
	wattrset(this, A_BOLD);
	wprintw(this, "Instructions:\n");
	wrefresh(this);
	return this;
}

void winstructions_free( WINDOW * w )
{
	delwin(w);
}
