#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"

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
	char (*board)[BOARD_WIDTH] = *this;

	/* Start from the bottom of the board and move up */
	for (int src_row = BOARD_HEIGHT - 1, dst_row = src_row; src_row >= 0; --src_row)
	{
		/* Check if the current row is full */
		bool is_full_row = true;
		for (int col = 0; col < BOARD_WIDTH; ++col)
		{
			if ( board[src_row][col] == 0 )
			{
				is_full_row = false;
				break;
			}
		}
		if (is_full_row)
		{
			++cleared_rows_count;
		}
		else
		{
			if (dst_row != src_row)
			{
				memcpy( board[dst_row], board[src_row], BOARD_WIDTH);
			}
			--dst_row;
		}
	}
	/* add empty rows at the top */
	memset( board[0], 0, BOARD_WIDTH * cleared_rows_count);

	return cleared_rows_count;
}
