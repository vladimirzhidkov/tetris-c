#ifndef BOARD_H
#define BOARD_H

#define BOARD_HEIGHT        15
#define BOARD_WIDTH         10
#define BOARD_LEN           BOARD_HEIGHT * BOARD_WIDTH 

typedef char board_t[BOARD_HEIGHT][BOARD_WIDTH];

board_t* board_create( void );
int board_clear_full_rows( board_t * );
void board_destroy( board_t * );

#endif /* BOARD_H */

