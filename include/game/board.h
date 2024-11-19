#ifndef BOARD_H
#define BOARD_H

#include "tetromino.h"
#include <stdbool.h>

#define BOARD_HEIGHT        15
#define BOARD_WIDTH         10
#define BOARD_LEN           BOARD_HEIGHT * BOARD_WIDTH 

typedef struct board_t board_t;
typedef char board_body_t[BOARD_HEIGHT][BOARD_WIDTH];

board_t* board_create( void );
void board_destroy( board_t * );

/* Actions */
int board_clear_full_rows( board_t * );
void board_fix_tetromino_to_board( board_t *, tetromino_t * );
bool board_detect_collision( board_t *, tetromino_t * );

/* Getters */
char* board_get_body( board_t *, tetromino_t * );

#endif /* BOARD_H */

