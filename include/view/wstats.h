#ifndef STATS_H
#define STATS_H

#include <ncurses.h>

#define WSTATS_HEIGHT	4
#define WSTATS_WIDTH	20

WINDOW * wstats_new( int y, int x );
void wstats_free( WINDOW * );

void wstats_update( WINDOW *, int level, int lines, int score );

#endif // STATS_H
