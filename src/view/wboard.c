#include <stdlib.h>
#include <ncurses.h>
#include <stdint.h>

#include "wboard.h"
#include "scaling.h"
#include "board_bytemap.h"

WINDOW * wboard_new( int y, int x )
{
	WINDOW *wboard_box = newwin( WBOARD_HEIGHT, WBOARD_WIDTH, y, x );
	box(wboard_box, 0, 0);
	wrefresh(wboard_box);
	delwin(wboard_box);
    
	WINDOW *this = newwin( FRAME_BOARD_HEIGHT, FRAME_BOARD_WIDTH, y + 1, x + 1 );
	wrefresh(this);
    return this;
}

void wboard_free( WINDOW * this )
{
    delwin(this);
}

void wboard_update( WINDOW *this, u_int8_t *board )
{
    u_int8_t *frame = malloc( FRAME_BOARD_LEN);
	scale_up( board, frame, BOARD_HEIGHT, BOARD_WIDTH );
	wmove(this, 0, 0);

	for (int i = 0; i < FRAME_BOARD_LEN; ++i)
	{
        int color_pair = frame[i] + 1;
		wattron(this, COLOR_PAIR(color_pair));
		waddch(this, ' ');
		wattroff(this, COLOR_PAIR(color_pair));
	}
	wrefresh(this);
}
