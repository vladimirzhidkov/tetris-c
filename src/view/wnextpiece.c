#include <stdlib.h>
#include <ncurses.h>

#include "wnextpiece.h"
#include "scaling.h"
#include "tetromino_bytemap.h"


WINDOW * wnextpiece_new( int y, int x )
{
	WINDOW *this = newwin(WNEXTPIECE_HEIGHT, WNEXTPIECE_WIDTH, y, x);
    return this;
}

void wnextpiece_free( WINDOW *this)
{
	delwin(this);
}


void wnextpiece_update( WINDOW *this, char* piece )
{
    char *frame = malloc( FRAME_NEXTPIECE_LEN );
    scale_up( piece, frame, TETROMINO_SIZE, TETROMINO_SIZE );

	wmove(this, 0, 0);
	wattrset(this, A_BOLD);
	waddstr(this, "Next");
	wmove(this, 1, 0);
	for (int i = 0; i < FRAME_NEXTPIECE_LEN; ++i)
	{
        int color_pair = frame[i] + 1;
		wattron(this, COLOR_PAIR(color_pair));
		waddch(this, ' ');
		wattroff(this, COLOR_PAIR(color_pair));
	}
	wrefresh(this);
    free(frame);
}
