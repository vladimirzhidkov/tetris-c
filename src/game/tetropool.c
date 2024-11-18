#include "tetropool.h"
#include "tetromino.h"

static tetromino_shape_t TETROPOOL[TETROPOOL_SIZE] =
	{ I_SHAPE, T_SHAPE, S_SHAPE, Z_SHAPE, O_SHAPE, L_SHAPE, J_SHAPE };

tetromino_shape_t* tetropool_get_shape( int index )
{
	return & TETROPOOL[index];
}
