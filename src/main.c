#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "view.h"
#include "shared.h"

#define CTRL_QUIT 'q'
#define DELAY_BASE_MS 1000
#define DELAY_REDUCTION_MS 90

Game* GAME;
View* VIEW;

void onEventStatsChanged( void )
{
	int level = getGameLevel( GAME );
	int clearedLinesCount = getGameClearedLinesCount( GAME );
	int score = getGameScore( GAME );

	renderStats(VIEW, level, clearedLinesCount, score);
}

void onEventBoardChanged( void )
{
	char* board = getBoard(GAME);

	renderBoard(VIEW, board);
}

void onEventPieceChanged( void )
{
	char* nextTetromino = getNextTetromino(GAME);
	renderNextPiece(VIEW, nextTetromino);
}

void onEventGameOver( void )
{
	destroyGame(GAME);
	destroyView(VIEW);
	puts("\nGame Over\n");
	exit(0);
}

void startGameLoop( void )
{
	while (1)
	{
		int level = getGameLevel( GAME );
		int delay = DELAY_BASE_MS - DELAY_REDUCTION_MS * level;
		long long elapsedTime = 0;
		long long startTime = currentTimeMs();

		while ( elapsedTime < delay )
		{
			int keyPressed = getch();
			if ( keyPressed != ERR )
			{
				switch (keyPressed)
				{
					case KEY_LEFT: moveLeft(GAME); break;
					case KEY_RIGHT: moveRight(GAME); break;
					case KEY_DOWN: moveDown(GAME); break;
					case KEY_UP: rotateClockwise(GAME); break;
					case CTRL_QUIT: onEventGameOver(); break;
				}
			}
			elapsedTime = currentTimeMs() - startTime;
		}
		moveDown(GAME);
	}
}

int main()
{
	GAME = createGame();
	registerEventHandler(GAME, EVENT_STATS_CHANGED, onEventStatsChanged);
	registerEventHandler(GAME, EVENT_BOARD_CHANGED, onEventBoardChanged);
	registerEventHandler(GAME, EVENT_PIECE_CHANGED, onEventPieceChanged);
	registerEventHandler(GAME, EVENT_GAME_OVER, onEventGameOver);

	VIEW = createView();
	renderNextPiece(VIEW, getNextTetromino(GAME));
	renderInstructions(VIEW, "q - quit");
	renderBoard(VIEW, getBoard(GAME));
	int level = getGameLevel( GAME );
	int clearedLinesCount = getGameClearedLinesCount( GAME );
	int score = getGameScore( GAME );
	renderStats(VIEW, level, clearedLinesCount, score);

	startGameLoop();
	return 0;
}