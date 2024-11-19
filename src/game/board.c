#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "board.h"

struct board_t
{
	board_body_t board; 
	board_body_t snapshot; // board with active tetromino merged with it
};

/* Fixes the tetromino to the board by setting occupied cells to 1.
 * The tetromino's position (x,y) determines where it is placed on the board.
 */
static void merge_tetromino_into_board_body( board_body_t board, tetromino_t *p_piece )
{
	tetromino_shape_t* piece = tetromino_get_active_shape( p_piece );
	int piece_x = tetromino_get_x( p_piece );
	int piece_y = tetromino_get_y( p_piece );

	for (int cell_y = 0; cell_y < TETROMINO_SIZE; ++cell_y)
	{
		for (int cell_x = 0; cell_x < TETROMINO_SIZE; ++cell_x)
		{
			if ( (*piece)[cell_y][cell_x] )
			{
				board[piece_y + cell_y][piece_x + cell_x] = 1;
			}
		}
	}
}


board_t* board_create( void )
{
	board_t* this = (board_t *)malloc(sizeof(board_t));
	memset( this, 0, sizeof(board_t) );
	return this;
}

void board_destroy( board_t *this )
{
	free(this);
}

/* Clears any full rows in the board and shifts rows down accordingly.
 * A row is considered full when all its cells are occupied.
 * Returns the number of rows that were cleared.
 */
int board_clear_full_rows( board_t *this )
{
	int cleared_rows_count = 0;

	/* Start from the bottom of the board and move up */
	for (int src_row = BOARD_HEIGHT - 1, dst_row = src_row; src_row >= 0; --src_row)
	{
		/* Check if the current row is full */
		bool is_full_row = true;
		for (int col = 0; col < BOARD_WIDTH; ++col)
		{
			if ( this->board[src_row][col] == 0 )
			{
				is_full_row = false;
				break;
			}
		}

		if (is_full_row)
		{
			++cleared_rows_count;
			continue;
		}

		if (dst_row != src_row)
		{
			memcpy( this->board[dst_row], this->board[src_row], BOARD_WIDTH);
		}
		--dst_row;
	}
	/* add empty rows at the top */
	memset( this->board[0], 0, BOARD_WIDTH * cleared_rows_count);

	return cleared_rows_count;
}

void board_fix_tetromino_to_board( board_t *this, tetromino_t *p_piece )
{
	merge_tetromino_into_board_body( this->board, p_piece );
}

bool board_detect_collision( board_t *this, tetromino_t *p_piece )
{
	tetromino_shape_t* piece = tetromino_get_active_shape( p_piece );
	int piece_x = tetromino_get_x( p_piece );
	int piece_y = tetromino_get_y( p_piece );

	for (int cell_y = 0; cell_y < TETROMINO_SIZE; ++cell_y)
	{
		for (int cell_x = 0; cell_x < TETROMINO_SIZE; ++cell_x)
		{
			if ( (*piece)[cell_y][cell_x] )
			{
				int board_x = piece_x + cell_x;
				int board_y = piece_y + cell_y;

				bool is_outside = board_x < 0 || board_x >= BOARD_WIDTH || board_y >= BOARD_HEIGHT;
				bool is_collided = this->board[board_y][board_x];

				if (is_outside || is_collided)
				{
					return true;
				}
			}
		}
	}
	return false;
}

char* board_generate_snapshot( board_t *this, tetromino_t *p_piece )
{
	memcpy(this->snapshot, this->board, BOARD_LEN);
	merge_tetromino_into_board_body( this->snapshot, p_piece );
	return (char *) this->snapshot;
}
