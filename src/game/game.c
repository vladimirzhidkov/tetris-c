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
	board_t *board;
	stats_t *stats;
	events_t *events;
};

static void spawn_piece( game_t* this )
{
	tetromino_spawn( this->tetromino, (BOARD_WIDTH - TETROMINO_SIZE) / 2, 0);
	bool is_collided = board_detect_collision( this->board, this->tetromino );

	if ( is_collided )
	{
		events_trigger( this->events, EVENT_GAME_OVER );
	}
	else
	{
		events_trigger( this->events, EVENT_NEXT_PIECE_CHANGED );
	}
}	
/*
 ******************** PUBLIC METHODS ******************** 
 */

game_t* game_create( void )
{
	game_t* this = malloc( sizeof(game_t) );
	this->tetromino = tetromino_create((BOARD_WIDTH - TETROMINO_SIZE) / 2, 0);
	this->board = board_create();
	this->stats = stats_create();
	this->events = events_create();
	return this;
}

void game_destroy( game_t* this )
{
	tetromino_destroy( this->tetromino );
	stats_destroy( this->stats );
	events_destroy( this->events );
	board_destroy( this->board );
	free(this);
}

/*
 ******************** ACTIONS ******************** 
 */

void game_move_piece_left( game_t* this )
{
	tetromino_move_left( this->tetromino );
	bool is_collided = board_detect_collision( this->board, this->tetromino );

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
	bool is_collided = board_detect_collision( this->board, this->tetromino );

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
	bool is_collided = board_detect_collision( this->board, this->tetromino );

	if ( is_collided )
	{
		tetromino_move_up( this->tetromino );
		board_fix_tetromino_to_board(this->board, this->tetromino);
		int lines_cleared = board_clear_full_rows(this->board);
		stats_update(this->stats, lines_cleared);
		events_trigger( this->events, EVENT_STATS_CHANGED );
		spawn_piece(this);
	}
	events_trigger( this->events, EVENT_BOARD_CHANGED );
}

void game_rotate_piece_cw( game_t* this )
{
	tetromino_rotate_cw( this->tetromino );
	bool is_collided = board_detect_collision( this->board, this->tetromino );

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
	bool is_collided = board_detect_collision( this->board, this->tetromino );

	if ( is_collided )
	{
		tetromino_rotate_cw( this->tetromino );
	}
	else
	{
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
}

void game_drop_piece( game_t* this )
{
	while ( !board_detect_collision( this->board, this->tetromino ) )
	{
		tetromino_move_down( this->tetromino );
		events_trigger( this->events, EVENT_BOARD_CHANGED );
	}
	tetromino_move_up( this->tetromino );
	board_fix_tetromino_to_board(this->board, this->tetromino);
	int lines_cleared = board_clear_full_rows(this->board);
	stats_update(this->stats, lines_cleared);
	events_trigger( this->events, EVENT_STATS_CHANGED );
	spawn_piece(this);
	events_trigger( this->events, EVENT_BOARD_CHANGED );
}





char* game_get_board( game_t* this )
{
	char* body = board_generate_snapshot(this->board, this->tetromino);
	return body;
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
	return stats_get_lines_cleared( this->stats );
}