#include "tetropool.h"
#include "tetromino.h"

static tetromino_t TETROPOOL[TETROPOOL_SIZE] =
	{ I_SHAPE, T_SHAPE, S_SHAPE, Z_SHAPE, O_SHAPE, L_SHAPE, J_SHAPE };

tetromino_t* tetropool_get_piece( int index )
{
	return & TETROPOOL[index];
}
