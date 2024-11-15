#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "shared.h"
#include "model.h"


// static const int SCORE_BASE_VALUE[5] =
// {
// 	0,
// 	SCORE_BASE * 1, /* 1 line(s) cleared */
// 	SCORE_BASE * 3, /* 2 line(s) cleared */
// 	SCORE_BASE * 5, /* 3 line(s) cleared */
// 	SCORE_BASE * 8  /* 4 line(s) cleared */
// };

static const TetroPool tetroPool =
{
	{ /* I shape */
		{0, 0, 0, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{ /* square shape */
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	},
	{ /* T shape */
		{0, 0, 0, 0},
		{1, 1, 1, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0}
	},
	{ /* S shape */
		{0, 0, 0, 0},
		{0, 0, 1, 1},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	},
	{ /* zigzag shape */
		{0, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	},
	{ /* J shape */
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	},
	{ /* L shape */
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	}
};


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
static void fixTetrominoToBoard( Board board, Tetromino tetromino, int tetrominoX, int tetrominoY )
{
	for (int cellY = 0; cellY < TETROMINO_SIZE; ++cellY)
	{
		for (int cellX = 0; cellX < TETROMINO_SIZE; ++cellX)
		{
			if ( tetromino[cellY][cellX] )
			{
				board[tetrominoY + cellY][tetrominoX + cellX] = 1;
			}
		}
	}
}

static bool detectCollision( Board board, Tetromino tetromino, int tetrominoX, int tetrominoY )
{
	/* iterate through tetrocells of the current tetromino */
	for (int cellY = 0; cellY < TETROMINO_SIZE; ++cellY)
	{
		for (int cellX = 0; cellX < TETROMINO_SIZE; ++cellX)
		{
			if ( tetromino[cellY][cellX] )
			{
				int boardX = tetrominoX + cellX;
				int boardY = tetrominoY + cellY;

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



static void incrementClearedLinesCount( Game* this, int increment )
{
	if (increment <= 0 || this->eventStatsChanged == NULL)
	{
		return;
	}
	this->clearedLinesCount += increment;
	this->eventStatsChanged();
}	


/*
 ******************** PUBLIC METHODS ******************** 
 */

Game* createGame( void )
{
	Game* this = malloc( sizeof(Game) );
	
	/* initialize board */
	memset( this->staticBoard, 0, BOARD_LEN );
	memset( this->activeBoard, 0, BOARD_LEN );

	/* initialize game stats */
	this->score = 0;
	this->level = 0;
	this->clearedLinesCount = 0;

	/* initialize random seed */
	srand(time(NULL));

	return this;
}

void spawnTetromino( Game* this )
{
	/* randomize the tetromino */
	int randomIndex = rand() % TETROPOOL_SIZE;
	memcpy(this->tetromino, tetroPool[randomIndex], TETROMINO_LEN);

	/* set the tetromino to the center of the board */
	int tetrominoX = (BOARD_WIDTH - TETROMINO_SIZE) / 2;
	int tetrominoY = 0;

	if ( !detectCollision( this->staticBoard, this->tetromino, tetrominoX, tetrominoY) )
	{
		this->tetrominoX = tetrominoX;
		this->tetrominoY = tetrominoY;
		this->eventBoardChanged();
	}
	else
	{
		this->eventGameOver();
	}
}

void moveLeft( Game* this )
{
	if ( !detectCollision(this->staticBoard, this->tetromino, this->tetrominoX - 1, this->tetrominoY) )
	{
		this->tetrominoX--;
		this->eventBoardChanged();
	}
}

void moveRight(Game* this)
{
	if ( !detectCollision(this->staticBoard, this->tetromino, this->tetrominoX + 1, this->tetrominoY) )
	{
		this->tetrominoX++;
		this->eventBoardChanged();
	}
}

void moveDown( Game* this )
{
	if ( !detectCollision(this->staticBoard, this->tetromino, this->tetrominoX, this->tetrominoY + 1) )
	{
		this->tetrominoY++;
		this->eventBoardChanged();
	}
	else
	{
		fixTetrominoToBoard(this->staticBoard, this->tetromino, this->tetrominoX, this->tetrominoY);
		int clearedLinesCount = clearFullRows(this->staticBoard);
		incrementClearedLinesCount(this, clearedLinesCount);
		spawnTetromino(this);
	}
}


void rotateClockwise( Game* this )
{
	transpose( (char *) this->tetromino, TETROMINO_SIZE);
	reverseRows( (char *) this->tetromino, TETROMINO_SIZE);

	if ( !detectCollision(this->staticBoard, this->tetromino, this->tetrominoX, this->tetrominoY) )
	{
		this->eventBoardChanged();
	}
	else
	{
		/* reverse changes */
		transpose( (char *) this->tetromino, TETROMINO_SIZE);
		reverseColumns( (char *) this->tetromino, TETROMINO_SIZE);
	}
}

void rotateCounterClockwise( Game* this )
{
	transpose( (char *) this->tetromino, TETROMINO_SIZE);
	reverseColumns( (char *) this->tetromino, TETROMINO_SIZE);

	if ( !detectCollision(this->activeBoard, this->tetromino, this->tetrominoX, this->tetrominoY))
	{
		this->eventBoardChanged();
	}
	else
	{
		/* reverse changes */
		transpose( (char *) this->tetromino, TETROMINO_SIZE);
		reverseRows( (char *) this->tetromino, TETROMINO_SIZE);
	}
}

Board* getBoard( Game* this )
{
	memcpy(this->activeBoard, this->staticBoard, BOARD_LEN);
	fixTetrominoToBoard(this->activeBoard, this->tetromino, this->tetrominoX, this->tetrominoY);
	return & this->activeBoard;
}

void destroyGame( Game* this )
{
	free(this);
}

