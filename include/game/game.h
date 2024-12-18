#ifndef GAME_H 
#define GAME_H

#include <stdint.h>
#include "events.h"

typedef struct game_t game_t;

game_t* game_new( void );
void game_free( game_t * );

/* actions */
void game_move_piece_left( game_t * );
void game_move_piece_right( game_t * );
void game_move_piece_down( game_t * );
void game_drop_piece( game_t * );
void game_rotate_piece_cw( game_t * );
void game_rotate_piece_ccw( game_t * );

/* getters */
u_int8_t * game_get_board( game_t * );
u_int8_t * game_get_next_shape( game_t * );

/* event handlers */
void game_register_event_handler( game_t *, event_t type, event_handler_fn handler );

/* stats */
int game_get_score( game_t *);
int game_get_level( game_t * );
int game_get_cleared_lines_count( game_t * );

#endif // GAME_H
