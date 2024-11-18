#ifndef TETROMINO_H
#define TETROMINO_H

#define TETROMINO_SIZE  4
#define TETROMINO_LEN   TETROMINO_SIZE * TETROMINO_SIZE

typedef char tetromino_t[TETROMINO_SIZE][TETROMINO_SIZE];

tetromino_t* tetromino_create( void );
void tetromino_rotate_cw( tetromino_t * );
void tetromino_rotate_ccw( tetromino_t * );
void tetromino_destroy( tetromino_t * );

#endif /* TETROMINO_H */

