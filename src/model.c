#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "shared.h"
#include "model.h"

static const int LINE_SCORE[] =
	{ 0, SCORE_LINE_1, SCORE_LINE_2, SCORE_LINE_3, SCORE_LINE_4 };

static Tetromino TETROPOOL[TETROPOOL_SIZE] =
	{ I_SHAPE, T_SHAPE, S_SHAPE, Z_SHAPE, O_SHAPE, L_SHAPE, J_SHAPE };

static int calcScore( int lines, int level )
{
	return LINE_SCORE[lines] * level;
}

/* Clears any full rows in the board and shifts rows down accordingly.
 * A row is considered full when all its cells are occupied.
 * Returns the number of rows that were cleared.
 */
static int clearFullRows( Board board )
{
	int clearedRowsCount = 0;

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
static void fixTetrominoToBoard( Board board, Tetromino* piece, int pieceX, int pieceY )
{
	for (int cellY = 0; cellY < TETROMINO_SIZE; ++cellY)
	{
		for (int cellX = 0; cellX < TETROMINO_SIZE; ++cellX)
		{
			if ( (*piece)[cellY][cellX] )
			{
				board[pieceY + cellY][pieceX + cellX] = 1;
			}
		}
	}
}

static bool detectCollision( Board board, Tetromino* piece, int pieceX, int pieceY )
{
	/* iterate through tetrocells of the current tetromino */
	for (int cellY = 0; cellY < TETROMINO_SIZE; ++cellY)
	{
		for (int cellX = 0; cellX < TETROMINO_SIZE; ++cellX)
		{
			if ( (*piece)[cellY][cellX] )
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
	this->piece = this->nextPiece;
	this->nextPiece = & TETROPOOL[rand() % TETROPOOL_SIZE];
	this->pieceX = (BOARD_WIDTH - TETROMINO_SIZE) / 2;
	this->pieceY = 0;

	if ( detectCollision( this->staticBoard, this->piece, this->pieceX, this->pieceY) )
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

	/* initialize random seed */
	srand(time(NULL));

	/* initialize tetromino */
	this->nextPiece = & TETROPOOL[rand() % TETROPOOL_SIZE];
	this->piece = & TETROPOOL[rand() % TETROPOOL_SIZE];
	this->pieceX = (BOARD_WIDTH - TETROMINO_SIZE) / 2;
	this->pieceY = 0;

	/* initialize board */
	memset( this->staticBoard, 0, BOARD_LEN );
	memset( this->activeBoard, 0, BOARD_LEN );

	/* initialize game stats */
	this->score = 0;
	this->level = 1;
	this->clearedLinesCount = 0;

	return this;
}

void moveLeft( Game* this )
{
	if ( !detectCollision(this->staticBoard, this->piece, this->pieceX - 1, this->pieceY) )
	{
		this->pieceX--;
		this->eventBoardChanged();
	}
}

void moveRight(Game* this)
{
	if ( !detectCollision(this->staticBoard, this->piece, this->pieceX + 1, this->pieceY) )
	{
		this->pieceX++;
		this->eventBoardChanged();
	}
}

void moveDown( Game* this )
{
	if ( !detectCollision(this->staticBoard, this->piece, this->pieceX, this->pieceY + 1) )
	{
		this->pieceY++;
		this->eventBoardChanged();
	}
	else
	{
		fixTetrominoToBoard(this->staticBoard, this->piece, this->pieceX, this->pieceY);
		int clearedLinesCount = clearFullRows(this->staticBoard);
		incrementClearedLinesCount(this, clearedLinesCount);
		spawnTetromino(this);
	}
}


void rotateClockwise( Game* this )
{
	transpose( (char *) this->piece, TETROMINO_SIZE);
	reverseRows( (char *) this->piece, TETROMINO_SIZE);

	if ( !detectCollision(this->staticBoard, this->piece, this->pieceX, this->pieceY) )
	{
		this->eventBoardChanged();
	}
	else
	{
		/* reverse changes */
		transpose( (char *) this->piece, TETROMINO_SIZE);
		reverseColumns( (char *) this->piece, TETROMINO_SIZE);
	}
}

void rotateCounterClockwise( Game* this )
{
	transpose( (char *) this->piece, TETROMINO_SIZE);
	reverseColumns( (char *) this->piece, TETROMINO_SIZE);

	if ( !detectCollision(this->activeBoard, this->piece, this->pieceX, this->pieceY))
	{
		this->eventBoardChanged();
	}
	else
	{
		/* reverse changes */
		transpose( (char *) this->piece, TETROMINO_SIZE);
		reverseRows( (char *) this->piece, TETROMINO_SIZE);
	}
}

char* getBoard( Game* this )
{
	memcpy(this->activeBoard, this->staticBoard, BOARD_LEN);
	fixTetrominoToBoard(this->activeBoard, this->piece, this->pieceX, this->pieceY);
	return (char *)this->activeBoard;
}

void destroyGame( Game* this )
{
	free(this);
}

