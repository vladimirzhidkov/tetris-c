#ifndef WINSTRUCTIONS_H
#define WINSTRUCTIONS_H

#include <ncurses.h>


#define WINSTRUCTIONS_HEIGHT 7
#define WINSTRUCTIONS_WIDTH 30

WINDOW * winstructions_new( int y, int x );
void winstructions_free( WINDOW * );

#endif // WINSTRUCTIONS_H
