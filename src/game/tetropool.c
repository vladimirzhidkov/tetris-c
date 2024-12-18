#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "tetropool.h"
#include "tetromino.h"

struct tetropool_t
{
	tetromino_bytemap_t pool[TETROPOOL_SIZE];
	int size;
};

tetropool_t* tetropool_new( void )
{
	srand(time(NULL));
	tetropool_t *this = (tetropool_t *)malloc( sizeof( tetropool_t ) );
	*this = (tetropool_t) {
		.pool = { I_SHAPE, T_SHAPE, S_SHAPE, Z_SHAPE, O_SHAPE, L_SHAPE, J_SHAPE },
		.size = TETROPOOL_SIZE
	};
	return this;
}

void tetropool_free( tetropool_t *this )
{
	free(this);
}

tetromino_bytemap_t* tetropool_get_random_shape( tetropool_t *this )
{
	int index = rand() % this->size;
	return &this->pool[index];
}
