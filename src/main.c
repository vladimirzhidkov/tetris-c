#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "game.h"
#include "view.h"


#define CTRL_QUIT 'q'
#define DELAY_BASE_MS 1000
#define DELAY_REDUCTION_MS 90

game_t* GAME;
View* VIEW;

long long current_time_ms( void )
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}

void onEventStatsChanged( void )
{
	int level = game_get_level( GAME );
	int clearedLinesCount = game_get_cleared_lines_count( GAME );
	int score = game_get_score( GAME );

	renderStats(VIEW, level, clearedLinesCount, score);
}

void onEventBoardChanged( void )
{
	char* board = game_get_board(GAME);

	renderBoard(VIEW, board);
}

void onEventPieceChanged( void )
{
	char* nextTetromino = game_get_next_piece(GAME);
	renderNextPiece(VIEW, nextTetromino);
}

void onEventGameOver( void )
{
	game_destroy(GAME);
	destroyView(VIEW);
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
					case KEY_DOWN: game_move_piece_down(GAME); break;
					case KEY_UP: game_rotate_piece_cw(GAME); break;
					case CTRL_QUIT: onEventGameOver(); break;
				}
			}
			elapsedTime = current_time_ms() - startTime;
		}
		game_move_piece_down(GAME);
	}
}

int main()
{
	GAME = game_create();
	game_register_event_handler(GAME, EVENT_STATS_CHANGED, onEventStatsChanged);
	game_register_event_handler(GAME, EVENT_BOARD_CHANGED, onEventBoardChanged);
	game_register_event_handler(GAME, EVENT_PIECE_CHANGED, onEventPieceChanged);
	game_register_event_handler(GAME, EVENT_GAME_OVER, onEventGameOver);

	VIEW = createView();
	renderNextPiece(VIEW, game_get_next_piece(GAME));
	renderInstructions(VIEW, "q - quit");
	renderBoard(VIEW, game_get_board(GAME));
	int level = game_get_level( GAME );
	int clearedLinesCount = game_get_cleared_lines_count( GAME );
	int score = game_get_score( GAME );
	renderStats(VIEW, level, clearedLinesCount, score);

	startGameLoop();
	return 0;
}