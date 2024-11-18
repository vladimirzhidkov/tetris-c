#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "stats.h"
#include "events.h"
#include "tetropool.h"
#include "tetromino.h"
#include "board.h"

struct game_t
{
	int next_piece_index;
	int piece_index;
	int piece_x;
	int piece_y;

	board_t* static_board; // board with tetrominoes fixed in place
	board_t* active_board; // board with tetromino that can move
	stats_t* stats;
	events_t* events;
};

/* Fixes the tetromino to the board by setting occupied cells to 1.
 * The tetromino's position (x,y) determines where it is placed on the board.
 */
static void fix_piece_to_board( board_t *p_board, tetromino_t *p_piece, int piece_x, int piece_y )
{
	for (int cell_y = 0; cell_y < TETROMINO_SIZE; ++cell_y)
	{
		for (int cell_x = 0; cell_x < TETROMINO_SIZE; ++cell_x)
		{
			if ( (*p_piece)[cell_y][cell_x] )
			{
				(*p_board)[piece_y + cell_y][piece_x + cell_x] = 1;
			}
		}
	}
}

static bool detect_collision( board_t *p_board, tetromino_t *p_piece, int piece_x, int piece_y )
{
	for (int cell_y = 0; cell_y < TETROMINO_SIZE; ++cell_y)
	{
		for (int cell_x = 0; cell_x < TETROMINO_SIZE; ++cell_x)
		{
			if ( (*p_piece)[cell_y][cell_x] )
			{
				int board_x = piece_x + cell_x;
				int board_y = piece_y + cell_y;

				bool is_outside = board_x < 0 || board_x >= BOARD_WIDTH || board_y >= BOARD_HEIGHT;
				bool is_collided = (*p_board)[board_y][board_x];

				if (is_outside || is_collided)
				{
					return true;
				}
			}
		}
	}
	return false;
}

static void spawn_piece( game_t* this )
{
	this->piece_index = this->next_piece_index;
	this->next_piece_index = rand() % TETROPOOL_SIZE;
	this->piece_x = (BOARD_WIDTH - TETROMINO_SIZE) / 2;
	this->piece_y = 0;
	tetromino_t* ptetromino = tetropool_get_piece(this->piece_index);
	bool is_collided = detect_collision( this->static_board, ptetromino, this->piece_x, this->piece_y);

	if ( is_collided )
	{
		events_trigger( this->events, EVENT_GAME_OVER );
	}
	else
	{
		events_trigger( this->events, EVENT_BOARD_CHANGED );
		events_trigger( this->events, EVENT_PIECE_CHANGED );
	}
}

static void increment_cleared_lines_count( game_t* this, int lines )
{
	if (lines <= 0)
	{
		return;
	}
	stats_update_cleared_lines_count(this->stats, lines);
	events_trigger( this->events, EVENT_STATS_CHANGED );
}	

/*
 ******************** PUBLIC METHODS ******************** 
 */

game_t* game_create( void )
{
	game_t* this = malloc( sizeof(game_t) );

	/* random seed */
	srand(time(NULL));

	/* tetromino */
	this->next_piece_index = rand() % TETROPOOL_SIZE;
	this->piece_index = rand() % TETROPOOL_SIZE;
	this->piece_x = (BOARD_WIDTH - TETROMINO_SIZE) / 2;
	this->piece_y = 0;

	this->static_board = board_create();
	this->active_board = board_create();
	this->stats = stats_create();
	this->events = events_create();

	return this;
}

void game_move_piece_left( game_t* this )
{
	tetromino_t* ptetromino = tetropool_get_piece(this->piece_index);
	bool is_collided = detect_collision( this->static_board, ptetromino, this->piece_x - 1, this->piece_y);

	if ( is_collided == false )
	{
		this->piece_x--;
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_move_piece_right(game_t* this)
{
	tetromino_t* ptetromino = tetropool_get_piece(this->piece_index);
	bool is_collided = detect_collision( this->static_board, ptetromino, this->piece_x + 1, this->piece_y);

	if ( is_collided == false )
	{
		this->piece_x++;
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_move_piece_down( game_t* this )
{
	tetromino_t* ptetromino = tetropool_get_piece(this->piece_index);
	bool is_collided = detect_collision( this->static_board, ptetromino, this->piece_x, this->piece_y + 1);

	if ( is_collided )
	{
		fix_piece_to_board(this->static_board, ptetromino, this->piece_x, this->piece_y);
		int clearedLinesCount = board_clear_full_rows(this->static_board);
		increment_cleared_lines_count(this, clearedLinesCount);
		spawn_piece(this);
	}
	else
	{
		this->piece_y++;
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_rotate_piece_cw( game_t* this )
{
	tetromino_t* ptetromino = tetropool_get_piece(this->piece_index);
	tetromino_rotate_cw( ptetromino );
	bool is_collided = detect_collision( this->static_board, ptetromino, this->piece_x, this->piece_y);

	if ( is_collided )
	{
		tetromino_rotate_ccw( ptetromino );
	}
	else
	{
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_rotate_piece_ccw( game_t* this )
{
	tetromino_t* tetromino = tetropool_get_piece(this->piece_index);
	tetromino_rotate_ccw( tetromino );
	bool is_collided = detect_collision( this->static_board, tetromino, this->piece_x, this->piece_y);

	if ( is_collided )
	{
		tetromino_rotate_cw( tetromino );
	}
	else
	{
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_destroy( game_t* this )
{
	stats_destroy( this->stats );
	events_destroy( this->events );
	board_destroy( this->static_board );
	board_destroy( this->active_board );
	free(this);
}

char* game_get_board( game_t* this )
{
	memcpy(this->active_board, this->static_board, BOARD_LEN);

	tetromino_t* p_piece = tetropool_get_piece(this->piece_index);

	fix_piece_to_board(this->active_board, p_piece, this->piece_x, this->piece_y);
	return (char *)this->active_board;
}

char* game_get_next_piece( game_t* this )
{
	tetromino_t* p_piece = tetropool_get_piece(this->next_piece_index);
	return (char *)p_piece;
}

void game_register_event_handler( game_t* this, event_t type, event_handler_fn handler )
{
	events_register( this->events, type, handler );
}



/*
 ******************** STATS ******************** 
 */

int game_get_score( game_t* this )
{
	return stats_get_score( this->stats );
}

int game_get_level( game_t* this )
{
	return stats_get_level( this->stats );
}

int game_get_cleared_lines_count( game_t* this )
{
	return stats_get_cleared_lines_count( this->stats );
}