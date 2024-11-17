#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "shared.h"
#include "model.h"

struct Game
{
	int nextPieceIndex;
	int pieceIndex;
	int pieceX;
	int pieceY;

	Board staticBoard; // board with tetrominoes fixed in place
	Board activeBoard; // board with tetromino that can move

	int score;
	int level;
	int clearedLinesCount;

	/* event handlers */
	EventHandler eventStatsChanged;
	EventHandler eventBoardChanged;
	EventHandler eventPieceChanged;
	EventHandler eventGameOver;
};


static const int LINE_SCORE[] =
	{ 0, SCORE_LINE_1, SCORE_LINE_2, SCORE_LINE_3, SCORE_LINE_4 };

static Tetromino TETROPOOL[TETROPOOL_SIZE] =
	{ I_SHAPE, T_SHAPE, S_SHAPE, Z_SHAPE, O_SHAPE, L_SHAPE, J_SHAPE };

static inline int calcScore(int lines, int level)
{
	return LINE_SCORE[lines] * level;
}

static inline Tetromino* getTetromino( Game* this )
{
	return & TETROPOOL[this->pieceIndex];
}

/* Clears any full rows in the board and shifts rows down accordingly.
 * A row is considered full when all its cells are occupied.
 * Returns the number of rows that were cleared.
 */
static int clearFullRows( Board *pboard )
{
	int clearedRowsCount = 0;
	char (*board)[BOARD_WIDTH] = *pboard;

	/* Start from the bottom of the board and move up */
	for (int srcRow = BOARD_HEIGHT - 1, dstRow = srcRow; srcRow >= 0; --srcRow)
	{
		/* Check if the current row is full */
		bool isFullRow = true;
		for (int col = 0; col < BOARD_WIDTH; ++col)
		{
			if ( board[srcRow][col] == 0 )
			{
				isFullRow = false;
				break;
			}
		}
		if (isFullRow)
		{
			++clearedRowsCount;
		}
		else
		{
			if (dstRow != srcRow)
			{
				memcpy( board[dstRow], board[srcRow], BOARD_WIDTH);
			}
			--dstRow;
		}
	}
	/* add empty rows at the top */
	memset( board[0], 0, BOARD_WIDTH * clearedRowsCount);

	return clearedRowsCount;
}

/* Fixes the tetromino to the board by setting occupied cells to 1.
 * The tetromino's position (x,y) determines where it is placed on the board.
 */
static void fixTetrominoToBoard( Board *pboard, Tetromino *ppiece, int pieceX, int pieceY )
{
	char (*board)[BOARD_WIDTH] = *pboard;
	char (*piece)[TETROMINO_SIZE] = *ppiece;

	for (int cellY = 0; cellY < TETROMINO_SIZE; ++cellY)
	{
		for (int cellX = 0; cellX < TETROMINO_SIZE; ++cellX)
		{
			if ( piece[cellY][cellX] )
			{
				board[pieceY + cellY][pieceX + cellX] = 1;
			}
		}
	}
}

static bool detectCollision( Board *pboard, Tetromino *ppiece, int pieceX, int pieceY )
{
	char (*board)[BOARD_WIDTH] = *pboard;
	char (*piece)[TETROMINO_SIZE] = *ppiece;

	/* iterate through tetrocells of the current tetromino */
	for (int cellY = 0; cellY < TETROMINO_SIZE; ++cellY)
	{
		for (int cellX = 0; cellX < TETROMINO_SIZE; ++cellX)
		{
			if ( piece[cellY][cellX] )
			{
				int boardX = pieceX + cellX;
				int boardY = pieceY + cellY;

				bool isOutside = boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT;
				bool isCollided = board[boardY][boardX];

				if (isOutside || isCollided)
				{
					return true;
				}
			}
		}
	}
	return false;
}

static void spawnTetromino( Game* this )
{
	this->pieceIndex = this->nextPieceIndex;
	this->nextPieceIndex = rand() % TETROPOOL_SIZE;
	this->pieceX = (BOARD_WIDTH - TETROMINO_SIZE) / 2;
	this->pieceY = 0;

	if ( detectCollision( & this->staticBoard, getTetromino(this), this->pieceX, this->pieceY) )
	{
		this->eventGameOver();
	}
	else
	{
		this->eventBoardChanged();
		this->eventPieceChanged();
	}
}

static void incrementClearedLinesCount( Game* this, int increment )
{
	if (increment <= 0 || this->eventStatsChanged == NULL)
	{
		return;
	}
	this->clearedLinesCount += increment;
	this->score += calcScore(increment, this->level);
	this->level = (this->clearedLinesCount / LINES_PER_LEVEL) + 1;
	this->eventStatsChanged();
}	

/*
 ******************** PUBLIC METHODS ******************** 
 */

Game* createGame( void )
{
	Game* this = malloc( sizeof(Game) );

	/* random seed */
	srand(time(NULL));

	/* tetromino */
	this->nextPieceIndex = rand() % TETROPOOL_SIZE;
	this->pieceIndex = rand() % TETROPOOL_SIZE;
	this->pieceX = (BOARD_WIDTH - TETROMINO_SIZE) / 2;
	this->pieceY = 0;

	/* board */
	memset( this->staticBoard, 0, BOARD_LEN );
	memset( this->activeBoard, 0, BOARD_LEN );

	/* game stats */
	this->score = 0;
	this->level = 1;
	this->clearedLinesCount = 0;

	return this;
}

void moveLeft( Game* this )
{
	if ( !detectCollision( & this->staticBoard, getTetromino(this), this->pieceX - 1, this->pieceY) )
	{
		this->pieceX--;
		this->eventBoardChanged();
	}
}

void moveRight(Game* this)
{
	if ( !detectCollision( & this->staticBoard, getTetromino(this), this->pieceX + 1, this->pieceY) )
	{
		this->pieceX++;
		this->eventBoardChanged();
	}
}

void moveDown( Game* this )
{
	Tetromino* tetromino = getTetromino(this);
	if ( !detectCollision( & this->staticBoard, tetromino, this->pieceX, this->pieceY + 1) )
	{
		this->pieceY++;
		this->eventBoardChanged();
	}
	else
	{
		fixTetrominoToBoard(& this->staticBoard, tetromino, this->pieceX, this->pieceY);
		int clearedLinesCount = clearFullRows(& this->staticBoard);
		incrementClearedLinesCount(this, clearedLinesCount);
		spawnTetromino(this);
	}
}

void rotateClockwise( Game* this )
{
	Tetromino* tetromino = getTetromino(this);
	transpose( (char *)tetromino, TETROMINO_SIZE);
	reverseRows( (char *)tetromino, TETROMINO_SIZE);

	if ( !detectCollision( & this->staticBoard, tetromino, this->pieceX, this->pieceY) )
	{
		this->eventBoardChanged();
	}
	else
	{
		transpose( (char *)tetromino, TETROMINO_SIZE);
		reverseColumns( (char *)tetromino, TETROMINO_SIZE);
	}
}

void rotateCounterClockwise( Game* this )
{
	Tetromino* tetromino = getTetromino(this);
	transpose( (char *)tetromino, TETROMINO_SIZE);
	reverseColumns( (char *)tetromino, TETROMINO_SIZE);

	if ( !detectCollision( & this->activeBoard, tetromino, this->pieceX, this->pieceY))
	{
		this->eventBoardChanged();
	}
	else
	{
		transpose( (char *)tetromino, TETROMINO_SIZE);
		reverseRows( (char *)tetromino, TETROMINO_SIZE);
	}
}

void destroyGame( Game* this )
{
	free(this);
}

/*
 ******************** GETTERS ******************** 
 */

char* getBoard( Game* this )
{
	Tetromino* tetromino = getTetromino(this);
	memcpy(this->activeBoard, this->staticBoard, BOARD_LEN);
	fixTetrominoToBoard(& this->activeBoard, tetromino, this->pieceX, this->pieceY);
	return (char *)this->activeBoard;
}

char* getNextTetromino( Game* this )
{
	return (char *)& TETROPOOL[this->nextPieceIndex];
}

int getScore( Game* this )
{
	return this->score;
}

int getLevel( Game* this )
{
	return this->level;
}

int getClearedLinesCount( Game* this )
{
	return this->clearedLinesCount;
}

/*
 ******************** EVENT HANDLERS ******************** 
 */

void registerEventHandler( Game* this, EventType type, EventHandler handler )
{
	switch (type)
	{
		case EVENT_STATS_CHANGED:
			this->eventStatsChanged = handler;
			break;
		case EVENT_BOARD_CHANGED:
			this->eventBoardChanged = handler;
			break;
		case EVENT_PIECE_CHANGED:
			this->eventPieceChanged = handler;
			break;
		case EVENT_GAME_OVER:
			this->eventGameOver = handler;
			break;
	}
}




