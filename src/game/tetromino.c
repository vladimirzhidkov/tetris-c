#include <time.h>
#include <stdlib.h>
#include "tetromino.h"
#include "tetropool.h"

struct tetromino_t
{
	tetromino_shape_t *active_shape;
	tetromino_shape_t *next_shape;
	int x;
	int y;
};

static void transpose( tetromino_shape_t *shape)
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

static void reverse_rows( tetromino_shape_t *shape)
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

static void reverse_cols( tetromino_shape_t *shape)
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

/*
 ******************** PUBLIC METHODS ******************** 
 */

tetromino_t* tetromino_create( int x, int y )
{
	srand(time(NULL));
	tetromino_t* this = (tetromino_t*)malloc( sizeof( tetromino_t ) );
	this->active_shape = tetropool_get_shape( rand() % TETROPOOL_SIZE );
	this->next_shape = tetropool_get_shape( rand() % TETROPOOL_SIZE );
	this->x = x;
	this->y = y;
	return this;
}

void tetromino_destroy( tetromino_t *this )
{
	free( this );
}

/*
 ******************** ACTIONS ******************** 
 */

void tetromino_spawn( tetromino_t *this, int x, int y )
{
	this->active_shape = this->next_shape;
	this->x = x;
	this->y = y;
	this->next_shape = tetropool_get_shape( rand() % TETROPOOL_SIZE );
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
	reverse_cols( tetromino->active_shape );
	reverse_rows( tetromino->active_shape );
}


/*
 ******************** GETTERS ******************** 
 */

int tetromino_get_x( tetromino_t *this )
{
	return this->x;
}

int tetromino_get_y( tetromino_t *this )
{
	return this->y;
}

tetromino_shape_t* tetromino_get_active_shape( tetromino_t *this )
{
	return this->active_shape;
}

tetromino_shape_t* tetromino_get_next_shape( tetromino_t *this )
{
	return this->next_shape;
}