#include <stdlib.h>
#include "tetromino.h"
#include "tetropool.h"

struct tetromino_t
{
	tetropool_t *tetropool;
	tetromino_bytemap_t *active_shape;
	tetromino_bytemap_t *next_shape;
	int x;
	int y;
	int size;
};

tetromino_t* tetromino_new( int x, int y )
{
	tetromino_t* this = (tetromino_t*)malloc( sizeof( tetromino_t ) );
	tetropool_t* pool = tetropool_new();
	*this = (tetromino_t) {
		.tetropool = pool,
		.active_shape = tetropool_get_random_shape(pool),
		.next_shape = tetropool_get_random_shape(pool),
		.x = x,
		.y = y,
		.size = TETROMINO_SIZE
    };
	return this;
}

void tetromino_free( tetromino_t *this )
{
	tetropool_free( this->tetropool );
	free( this );
}



static void transpose( tetromino_bytemap_t *shape)
{
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = i + 1; j < TETROMINO_SIZE; j++)
		{
			char temp = (*shape)[i][j];
			(*shape)[i][j] = (*shape)[j][i];
			(*shape)[j][i] = temp;
		}
	}
}

static void reverse_rows( tetromino_bytemap_t *shape)
{
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = 0, k = TETROMINO_SIZE - 1; j < k; j++, k--)
		{
			char temp = (*shape)[i][j];
			(*shape)[i][j] = (*shape)[i][k];
			(*shape)[i][k] = temp;
		}
	}
}

static void reverse_cols( tetromino_bytemap_t *shape)
{
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = 0, k = TETROMINO_SIZE - 1; j < k; j++, k--)
		{
			char temp = (*shape)[j][i];
			(*shape)[j][i] = (*shape)[k][i];
			(*shape)[k][i] = temp;
		}
	}
}

void tetromino_spawn( tetromino_t *this, int x, int y )
{
	this->active_shape = this->next_shape;
	this->x = x;
	this->y = y;
	this->next_shape = tetropool_get_random_shape( this->tetropool );
}

void tetromino_move_down( tetromino_t *this )
{
	this->y++;
}

void tetromino_move_up( tetromino_t *this )
{
	this->y--;
}

void tetromino_move_left( tetromino_t *this )
{
	this->x--;
}

void tetromino_move_right( tetromino_t *this )
{
	this->x++;
}

void tetromino_rotate_cw( tetromino_t *tetromino)
{
	transpose( tetromino->active_shape );
	reverse_rows( tetromino->active_shape );
}

void tetromino_rotate_ccw( tetromino_t *tetromino)
{
	transpose( tetromino->active_shape );
	reverse_cols( tetromino->active_shape );
}



int tetromino_get_x( tetromino_t *this )
{
	return this->x;
}

int tetromino_get_y( tetromino_t *this )
{
	return this->y;
}

tetromino_bytemap_t* tetromino_get_active_shape( tetromino_t *this )
{
	return this->active_shape;
}

tetromino_bytemap_t* tetromino_get_next_shape( tetromino_t *this )
{
	return this->next_shape;
}
