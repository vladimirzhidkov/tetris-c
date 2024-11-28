#ifndef TETROPOOL_H
#define TETROPOOL_H

#include "tetromino_bytemap.h"

/* tetromino pool */
#define TETROPOOL_SIZE 7
#define I_SHAPE { \
	{0, 0, 0, 0}, \
	{1, 1, 1, 1}, \
	{0, 0, 0, 0}, \
	{0, 0, 0, 0}  }
#define T_SHAPE { \
	{0, 0, 0, 0}, \
	{1, 1, 1, 0}, \
	{0, 1, 0, 0}, \
	{0, 0, 0, 0}  }
#define S_SHAPE { \
	{0, 0, 0, 0}, \
	{0, 0, 1, 1}, \
	{0, 1, 1, 0}, \
	{0, 0, 0, 0}  }
#define Z_SHAPE { \
	{0, 0, 0, 0}, \
	{0, 1, 1, 0}, \
	{0, 0, 1, 1}, \
	{0, 0, 0, 0}  }
#define O_SHAPE { \
	{0, 0, 0, 0}, \
	{0, 1, 1, 0}, \
	{0, 1, 1, 0}, \
	{0, 0, 0, 0}  }
#define L_SHAPE { \
	{0, 1, 0, 0}, \
	{0, 1, 0, 0}, \
	{0, 1, 1, 0}, \
	{0, 0, 0, 0}  }
#define J_SHAPE { \
	{0, 0, 1, 0}, \
	{0, 0, 1, 0}, \
	{0, 1, 1, 0}, \
	{0, 0, 0, 0}  }

typedef struct tetropool_t tetropool_t;

tetropool_t* tetropool_new( void );
void tetropool_free( tetropool_t * );

/* Actions */
tetromino_bytemap_t* tetropool_get_random_shape( tetropool_t * );

#endif /* TETROPOOL_H */



