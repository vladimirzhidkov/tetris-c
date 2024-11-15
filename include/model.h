#ifndef GAME_H 
#define GAME_H

#include "shared.h"

/* scoring rules */
#define LINES_PER_LEVEL 10
#define SCORE_BASE 10 /* base points for clearing a line */

/* tetromino */
#define TETROMINO_SIZE      4
#define TETROMINO_LEN       TETROMINO_SIZE * TETROMINO_SIZE
typedef char Tetromino[TETROMINO_SIZE][TETROMINO_SIZE];

/* tetromino pool */
#define TETROPOOL_SIZE 7
typedef char TetroPool[TETROPOOL_SIZE][TETROMINO_SIZE][TETROMINO_SIZE];

/* board */
#define BOARD_HEIGHT        15
#define BOARD_WIDTH         10
#define BOARD_LEN           BOARD_HEIGHT * BOARD_WIDTH 
typedef char Board[BOARD_HEIGHT][BOARD_WIDTH];

/* game */
typedef struct Game
{
	Tetromino tetromino;
	int tetrominoX;
	int tetrominoY;

	Board staticBoard; // board with tetrominoes fixed in place
	Board activeBoard; // board with tetrominoes that can move

	int score;
	int level;
	int clearedLinesCount;

	/* observers */
	void (* eventStatsChanged)( void );
	void (* eventBoardChanged)( void );
	void (* eventGameOver)( void );
} Game;

Game* createGame( void );
void spawnTetromino( Game* this );
void moveLeft( Game* this );
void moveRight( Game* this );
void moveDown( Game* this );
void rotateClockwise( Game* this );
void rotateCounterClockwise( Game* this );
Board* getBoard( Game* this );
void destroyGame( Game* this );

#endif // GAME_H