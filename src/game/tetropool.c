#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "tetropool.h"
#include "tetromino.h"

struct tetropool_t
{
	tetromino_shape_t pool[TETROPOOL_SIZE];
	int size;
};

tetropool_t* tetropool_create( void )
{
	srand(time(NULL));
	tetropool_t *this = (tetropool_t *)malloc( sizeof( tetropool_t ) );
	tetromino_shape_t pool[TETROPOOL_SIZE] =
	{
		 I_SHAPE,
		 T_SHAPE,
		 S_SHAPE,
		 Z_SHAPE,
		 O_SHAPE,
		 L_SHAPE,
		 J_SHAPE
	};
	memcpy(this->pool, pool, sizeof(tetromino_shape_t) * TETROPOOL_SIZE);
	this->size = TETROPOOL_SIZE;
	return this;
}

void tetropool_destroy( tetropool_t *this )
{
	free(this);
}

tetromino_shape_t* tetropool_get_random_shape( tetropool_t *this )
{
	int index = rand() % this->size;
	return &this->pool[index];
}
