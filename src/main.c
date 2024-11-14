#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "model.h"
#include "view.h"

#define CTRL_MOVELEFT 'a'
#define CTRL_MOVERIGHT 'd'
#define CTRL_MOVEDOWN 's'
#define CTRL_ROTATE 'w'
#define CTRL_QUIT 'q'

#define UPDATE_RATE_BASE_MS 1000
#define UPDATE_RATE_REDUCTION_MS 90

void startGameLoop( Game* g, View* v );

static long long current_time_ms()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}

void exitGame(Game* g, View* v)
{
	destroyGame(g);
	destroyView(v);
	puts("\nGame Over\n");
	exit(0);
}

int main()
{
	Game* g = createGame();
	View* v = createView();
	spawnTetromino(g);
	renderScore(v, g->level, g->clearedLinesCount, g->score);
	renderBoard(v, getBoardFrame(g));
	startGameLoop(g, v);
	return 0;
}

void startGameLoop( Game* g, View* v )
{
	while (1)
	{
		long long last_updated_time_ms = current_time_ms();

		// loop for a bit, constantly reading from stdin
		int update_rate = UPDATE_RATE_BASE_MS - UPDATE_RATE_REDUCTION_MS * g->level;

		while (current_time_ms() - last_updated_time_ms < update_rate)
		{
			int key;

			/* read input from stdin */
			if ( (key = getch()) != ERR)
			{
				switch (key)
				{
					case CTRL_MOVELEFT: moveLeft(g); break;
					case CTRL_MOVERIGHT: moveRight(g); break;
					case CTRL_MOVEDOWN: moveDown(g); break;
					case CTRL_ROTATE: rotateClockwise(g); break;
					case CTRL_QUIT: exitGame(g, v); break;
					default: /* handle unexpected input */ break;
				}
				renderBoard(v, getBoardFrame(g));
				renderScore(v, g->level, g->clearedLinesCount, g->score);
			}
		}

		// drop the tetromino by one step
		moveDown(g);
		renderBoard(v, getBoardFrame(g));
		renderScore(v, g->level, g->clearedLinesCount, g->score);
	}
}

