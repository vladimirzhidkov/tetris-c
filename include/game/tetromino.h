#ifndef TETROMINO_H
#define TETROMINO_H

#include "tetromino_bytemap.h"
#include "tetropool.h"

typedef struct tetromino_t tetromino_t;

tetromino_t* tetromino_new( int x, int y );
void tetromino_free( tetromino_t * );

/* Actions */
void tetromino_spawn( tetromino_t *, int x, int y );
void tetromino_move_down( tetromino_t * );
void tetromino_move_up( tetromino_t * );
void tetromino_move_left( tetromino_t * );
void tetromino_move_right( tetromino_t * );
void tetromino_rotate_cw( tetromino_t * );
void tetromino_rotate_ccw( tetromino_t * );

/* Getters */
tetromino_bytemap_t* tetromino_get_active_shape( tetromino_t * );
tetromino_bytemap_t* tetromino_get_next_shape( tetromino_t * );
int tetromino_get_x( tetromino_t * );
int tetromino_get_y( tetromino_t * );

#endif /* TETROMINO_H */

