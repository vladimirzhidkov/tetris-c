#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "stats.h"
#include "events.h"
#include "tetromino.h"
#include "board.h"

struct game_t
{
	tetromino_t *tetromino;
	board_t *static_board; // board with tetrominoes fixed in place
	board_t *active_board; // board with tetromino that can move
	stats_t *stats;
	events_t *events;
};

/* Fixes the tetromino to the board by setting occupied cells to 1.
 * The tetromino's position (x,y) determines where it is placed on the board.
 */
static void fix_piece_to_board( board_t *p_board, tetromino_t *p_piece )
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
				(*p_board)[piece_y + cell_y][piece_x + cell_x] = 1;
			}
		}
	}
}

static bool detect_collision( board_t *p_board, tetromino_t *p_piece )
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

static void increment_cleared_lines_count( game_t* this, int lines )
{
	if (lines <= 0)
	{
		return;
	}
	stats_update_cleared_lines_count(this->stats, lines);
	events_trigger( this->events, EVENT_STATS_CHANGED );
}	

static void spawn_piece( game_t* this )
{
	tetromino_spawn( this->tetromino, (BOARD_WIDTH - TETROMINO_SIZE) / 2, 0);
	bool is_collided = detect_collision( this->static_board, this->tetromino );

	if ( is_collided )
	{
		events_trigger( this->events, EVENT_GAME_OVER );
	}
	else
	{
		events_trigger( this->events, EVENT_PIECE_CHANGED );
	}
}	
/*
 ******************** PUBLIC METHODS ******************** 
 */

game_t* game_create( void )
{
	game_t* this = malloc( sizeof(game_t) );
	this->tetromino = tetromino_create((BOARD_WIDTH - TETROMINO_SIZE) / 2, 0);
	this->active_board = board_create();
	this->static_board = board_create();
	this->stats = stats_create();
	this->events = events_create();
	return this;
}

void game_destroy( game_t* this )
{
	tetromino_destroy( this->tetromino );
	stats_destroy( this->stats );
	events_destroy( this->events );
	board_destroy( this->active_board );
	board_destroy( this->static_board );
	free(this);
}

/*
 ******************** ACTIONS ******************** 
 */

void game_move_piece_left( game_t* this )
{
	tetromino_move_left( this->tetromino );
	bool is_collided = detect_collision( this->static_board, this->tetromino );

	if ( is_collided )
	{
		tetromino_move_right( this->tetromino );
	}
	else
	{
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_move_piece_right(game_t* this)
{
	tetromino_move_right( this->tetromino );
	bool is_collided = detect_collision( this->static_board, this->tetromino );

	if ( is_collided )
	{
		tetromino_move_left( this->tetromino );
	}
	else
	{
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_move_piece_down( game_t* this )
{
	tetromino_move_down( this->tetromino );
	bool is_collided = detect_collision( this->static_board, this->tetromino );

	if ( is_collided )
	{
		tetromino_move_up( this->tetromino );
		fix_piece_to_board(this->static_board, this->tetromino);
		int clearedLinesCount = board_clear_full_rows(this->static_board);
		increment_cleared_lines_count(this, clearedLinesCount);
		spawn_piece(this);
	}
	events_trigger( this->events, EVENT_BOARD_CHANGED );
}

void game_rotate_piece_cw( game_t* this )
{
	tetromino_rotate_cw( this->tetromino );
	bool is_collided = detect_collision( this->static_board, this->tetromino );

	if ( is_collided )
	{
		tetromino_rotate_ccw( this->tetromino );
	}
	else
	{
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_rotate_piece_ccw( game_t* this )
{
	tetromino_rotate_ccw( this->tetromino );
	bool is_collided = detect_collision( this->static_board, this->tetromino );

	if ( is_collided )
	{
		tetromino_rotate_cw( this->tetromino );
	}
	else
	{
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

char* game_get_board( game_t* this )
{
	memcpy(this->active_board, this->static_board, BOARD_LEN);
	fix_piece_to_board(this->active_board, this->tetromino);
	return (char *)this->active_board;
}

char* game_get_next_shape( game_t* this )
{
	char *shape = (char *)tetromino_get_next_shape(this->tetromino);
	return shape;
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