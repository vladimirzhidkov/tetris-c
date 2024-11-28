#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>

#include "wboard.h"
#include "wnextpiece.h"
#include "wstats.h"
#include "winstructions.h"

#define WBOARD_Y	    	0
#define WBOARD_X	        0	

#define WNEXTPIECE_Y		1
#define WNEXTPIECE_X		WBOARD_X + WBOARD_WIDTH

#define WSTATS_Y		    WNEXTPIECE_Y + WNEXTPIECE_HEIGHT 
#define WSTATS_X	    	WBOARD_X + WBOARD_WIDTH

#define WINSTRUCTIONS_Y	    WSTATS_Y + WSTATS_HEIGHT
#define WINSTRUCTIONS_X		WBOARD_X + WBOARD_WIDTH

typedef struct view_t view_t;

view_t* view_new( void );
void view_free( view_t * );

void update_board( view_t *, char* board );
void update_nextpiece( view_t *, char* piece );
void update_stats( view_t *, int level, int lines, int score );

#endif // VIEW_H
