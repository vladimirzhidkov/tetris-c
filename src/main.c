#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include "game.h"
#include "view.h"

#define CTRL_QUIT 'q'
#define CTRL_DROP ' '

#define DELAY_BASE_MS 1000
#define DELAY_REDUCTION_MS 90

game_t* GAME;
view_t* VIEW;

long long current_time_ms( void )
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}

void onEventStatsChanged( void )
{
	int level = game_get_level( GAME );
	int lines_cleared = game_get_cleared_lines_count( GAME );
	int score = game_get_score( GAME );

	update_stats(VIEW, level, lines_cleared, score);
}

void onEventBoardChanged( void )
{
	u_int8_t *board = game_get_board(GAME);
	update_board(VIEW, board);
}

void onEventNextPieceChanged( void )
{
	u_int8_t *nextTetromino = game_get_next_shape(GAME);
	update_nextpiece(VIEW, nextTetromino);
}

void onEventGameOver( void )
{
	game_free(GAME);
	view_free(VIEW);
	puts("\nGame Over\n");
	exit(0);
}

void startGameLoop( void )
{
	while (1)
	{
		int level = game_get_level( GAME );
		int delay = DELAY_BASE_MS - DELAY_REDUCTION_MS * level;
		long long elapsedTime = 0;
		long long startTime = current_time_ms();

		while ( elapsedTime < delay )
		{
			int keyPressed = getch();
			if ( keyPressed != ERR )
			{
				switch (keyPressed)
				{
					case KEY_LEFT: game_move_piece_left(GAME); break;
					case KEY_RIGHT: game_move_piece_right(GAME); break;
					case KEY_UP: game_rotate_piece_cw(GAME); break;
					case KEY_DOWN: game_move_piece_down(GAME); break;
					case CTRL_DROP: game_drop_piece(GAME); break;
					case CTRL_QUIT: onEventGameOver(); break;
				}
			}
			elapsedTime = current_time_ms() - startTime;
		}
		game_move_piece_down(GAME);
	}
}

int main( void )
{
	GAME = game_new();
	game_register_event_handler(GAME, EVENT_STATS_CHANGED, onEventStatsChanged);
	game_register_event_handler(GAME, EVENT_BOARD_CHANGED, onEventBoardChanged);
	game_register_event_handler(GAME, EVENT_NEXT_PIECE_CHANGED, onEventNextPieceChanged);
	game_register_event_handler(GAME, EVENT_GAME_OVER, onEventGameOver);
	int level = game_get_level( GAME );
	int lines_cleared = game_get_cleared_lines_count( GAME );
	int score = game_get_score( GAME );

	VIEW = view_new();
	update_board(VIEW, game_get_board(GAME));
	update_nextpiece(VIEW, game_get_next_shape(GAME));
	update_stats(VIEW, level, lines_cleared, score);

	startGameLoop();
	return 0;
}
