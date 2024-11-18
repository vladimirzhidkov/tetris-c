#include <stdlib.h>
#include "tetromino.h"

struct tetromino_t
{
	int tetropool_index;
	int x;
	int y;
};

static void transpose( tetromino_t *piece)
{
	char (*p)[TETROMINO_SIZE] = (char (*)[TETROMINO_SIZE])piece;
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = i + 1; j < TETROMINO_SIZE; j++)
		{
			char temp = p[i][j];
			p[i][j] = p[j][i];
			p[j][i] = temp;
		}
	}
}

static void reverse_rows( tetromino_t *piece)
{
	char (*p)[TETROMINO_SIZE] = (char (*)[TETROMINO_SIZE])piece;
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = 0, k = TETROMINO_SIZE - 1; j < k; j++, k--)
		{
			char temp = p[i][j];
			p[i][j] = p[i][k];
			p[i][k] = temp;
		}
	}
}

static void reverse_cols( tetromino_t *piece)
{
	char (*p)[TETROMINO_SIZE] = (char (*)[TETROMINO_SIZE])piece;
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = 0, k = TETROMINO_SIZE - 1; j < k; j++, k--)
		{
			char temp = p[j][i];
			p[j][i] = p[k][i];
			p[k][i] = temp;
		}
	}
}

void tetromino_rotate_cw( tetromino_t *piece)
{
	transpose( piece );
	reverse_rows( piece );
}

void tetromino_rotate_ccw( tetromino_t *piece)
{
	reverse_cols( piece );
	reverse_rows( piece );
}

tetromino_t* tetromino_create( void )
{
	tetromino_t* this = (tetromino_t*)malloc( sizeof( tetromino_t ) );

	// this->tetropool_index = 0;
	// this->x = 0;
	// this->y = 0;

	return this;
}

void tetromino_destroy( tetromino_t *this )
{
	free( this );
}
