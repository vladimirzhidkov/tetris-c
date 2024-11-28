#ifndef WINSTRUCTIONS_H
#define WINSTRUCTIONS_H

#include <ncurses.h>


#define WINSTRUCTIONS_HEIGHT 5
#define WINSTRUCTIONS_WIDTH 20

WINDOW * winstructions_new( int y, int x );
void winstructions_free( WINDOW * );

#endif // WINSTRUCTIONS_H
