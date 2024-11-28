#ifndef BOARD_H
#define BOARD_H
#include <stdbool.h>
#include "board_bytemap.h"
#include "tetromino.h"

typedef struct board_t board_t;

board_t * board_new( void );
void board_free( board_t * );

/* Actions */
int board_clear_full_rows( board_t * );
void board_fix_tetromino_to_board( board_t *, tetromino_t * );
bool board_detect_collision( board_t *, tetromino_t * );
board_bytemap_t* board_generate_snapshot( board_t *, tetromino_t * );

#endif /* BOARD_H */

