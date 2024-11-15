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
	renderGameStats(VIEW, GAME->level, GAME->clearedLinesCount, GAME->score);
}

void onEventBoardChanged( void )
{
	renderBoard(VIEW, getBoard(GAME));
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
		int delay = DELAY_BASE_MS - DELAY_REDUCTION_MS * GAME->level;
		long long startTime = currentTimeMs();
		long long elapsedTime = currentTimeMs() - startTime;

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
	GAME->eventStatsChanged = onEventStatsChanged;
	GAME->eventBoardChanged = onEventBoardChanged;
	GAME->eventGameOver = onEventGameOver;
	VIEW = createView();
	spawnTetromino(GAME);
	renderGameStats(VIEW, GAME->level, GAME->clearedLinesCount, GAME->score);
	renderBoard(VIEW, getBoard(GAME));
	startGameLoop();
	return 0;
}