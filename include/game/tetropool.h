#ifndef TETROPOOL_H
#define TETROPOOL_H

#include "tetromino.h"

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

tetromino_shape_t* tetropool_get_shape( int index );

#endif /* TETROPOOL_H */