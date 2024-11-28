#include <ncurses.h>
#include "winstructions.h"


WINDOW * winstructions_new( int y, int x )
{
	WINDOW* this = newwin(WINSTRUCTIONS_HEIGHT, WINSTRUCTIONS_WIDTH, y, x);
	wattrset(this, A_BOLD);
    wmove(this, 0, 0);
	// wprintw(this, "Instructions:\n");
	waddstr(this, "Instructions:\n");
	waddstr(this, "left arrow  - move left\n");
	waddstr(this, "right arrow - move right\n");
	waddstr(this, "down arrow  - move down\n");
	waddstr(this, "up arrow    - rotate\n");
	waddstr(this, "space       - drop\n");
	waddstr(this, "q           - quit\n");
	wrefresh(this);
	return this;
}

void winstructions_free( WINDOW * w )
{
	delwin(w);
}
