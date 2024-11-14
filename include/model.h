#ifndef GAME_H 
#define GAME_H

#include "shared.h"

#define TETROPOOL_SIZE 7
#define LINES_PER_LEVEL 10
#define SCORE_BASE 10 /* base points for clearing a line */

typedef struct Game
{
	char tetromino[TETROMINO_SIZE][TETROMINO_SIZE];
	int tetrominoX;
	int tetrominoY;

	char board[BOARD_HEIGHT][BOARD_WIDTH];
	char frame[BOARD_HEIGHT][BOARD_WIDTH];

	int score;
	int level;
	int clearedLinesCount;
} Game;

Game* createGame( void );
void spawnTetromino( Game* this );
void moveLeft( Game* this );
void moveRight( Game* this );
void moveDown( Game* this );
void rotateClockwise( Game* this );
void rotateCounterClockwise( Game* this );
char* getBoardFrame( Game* this );
void gameOver( Game* this );
void destroyGame( Game* this );

#endif // GAME_H