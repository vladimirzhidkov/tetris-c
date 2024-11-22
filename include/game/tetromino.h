#ifndef TETROMINO_H
#define TETROMINO_H

#define TETROMINO_SIZE  4
#define TETROMINO_LEN   TETROMINO_SIZE * TETROMINO_SIZE

typedef char tetromino_shape_t[TETROMINO_SIZE][TETROMINO_SIZE];
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
tetromino_shape_t* tetromino_get_active_shape( tetromino_t * );
tetromino_shape_t* tetromino_get_next_shape( tetromino_t * );
int tetromino_get_x( tetromino_t * );
int tetromino_get_y( tetromino_t * );

#endif /* TETROMINO_H */

