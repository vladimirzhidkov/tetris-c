#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

static const char tetroPool[TETROPOOL_SIZE][TETROMINO_SIZE][TETROMINO_SIZE] =
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


static void transpose( char tetromino[][TETROMINO_SIZE] )
{
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = i + 1; j < TETROMINO_SIZE; j++)
		{
			char temp = tetromino[i][j];
			tetromino[i][j] = tetromino[j][i];
			tetromino[j][i] = temp;
		}
	}
}

static void reverseRows( char tetromino[][TETROMINO_SIZE] )
{
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = 0, k = TETROMINO_SIZE - 1; j < k; j++, k--)
		{
			char temp = tetromino[i][j];
			tetromino[i][j] = tetromino[i][k];
			tetromino[i][k] = temp;
		}
	}
}

static void reverseColumns( char tetromino[][TETROMINO_SIZE] )
{
	for (int i = 0; i < TETROMINO_SIZE; i++)
	{
		for (int j = 0, k = TETROMINO_SIZE - 1; j < k; j++, k--)
		{
			char temp = tetromino[j][i];
			tetromino[j][i] = tetromino[k][i];
			tetromino[k][i] = temp;
		}
	}
}


/* Clears any full rows in the board and shifts rows down accordingly.
 * A row is considered full when all its cells are occupied.
 * Returns the number of rows that were cleared.
 */
static int clearFullRows( char board[][BOARD_WIDTH] )
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
static void fixTetrominoToBoard( char board[][BOARD_WIDTH], char tetromino[][TETROMINO_SIZE], int tetrominoX, int tetrominoY )
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

static bool detectCollision( char board[][BOARD_WIDTH], char tetromino[][TETROMINO_SIZE], int tetrominoX, int tetrominoY )
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




/*********************************************************************/
/*************************** GAME METHODS ****************************/
/*********************************************************************/


Game* createGame( void )
{
	Game* this = malloc( sizeof(Game) );

	/* initialize tetromino and its position */
	memset( this->tetromino, 0, TETROMINO_LEN );
	this->tetrominoX = 0;
	this->tetrominoY = 0;
	
	/* initialize board */
	memset( this->board, 0, BOARD_LEN );

	/* initialize game stats */
	this->score = 0;
	this->level = 0;
	this->clearedLinesCount = 0;

	return this;
}

void spawnTetromino( Game* this )
{
	/* randomize the tetromino */
	int randomIndex = rand() % TETROPOOL_SIZE;
	memcpy(this->tetromino, tetroPool[randomIndex], TETROMINO_LEN);

	/* set the tetromino to the center of the board */
	this->tetrominoX = (BOARD_WIDTH - TETROMINO_SIZE) / 2;
	this->tetrominoY = 0;

	if ( detectCollision(this->board, this->tetromino, this->tetrominoX, this->tetrominoY) )
	{
		gameOver(this);
	}
}

void moveLeft( Game* this )
{
	if ( !detectCollision(this->board, this->tetromino, this->tetrominoX - 1, this->tetrominoY) )
	{
		this->tetrominoX--;
	}
}

void moveRight(Game* this)
{
	if ( !detectCollision(this->board, this->tetromino, this->tetrominoX + 1, this->tetrominoY) )
	{
		this->tetrominoX++;
	}
}

void moveDown( Game* this )
{
	if ( detectCollision(this->board, this->tetromino, this->tetrominoX, this->tetrominoY + 1) )
	{
		fixTetrominoToBoard(this->board, this->tetromino, this->tetrominoX, this->tetrominoY);
		this->clearedLinesCount += clearFullRows(this->board);
		spawnTetromino(this);
	}
	else
	{
		this->tetrominoY++;
	}
}


void rotateClockwise( Game* this )
{
	transpose(this->tetromino);
	reverseRows(this->tetromino);

	if (detectCollision(this->board, this->tetromino, this->tetrominoX, this->tetrominoY))
	{
		transpose(this->tetromino);
		reverseColumns(this->tetromino);
	}
}

void rotateCounterClockwise( Game* this )
{
	transpose(this->tetromino);
	reverseColumns(this->tetromino);

	if (detectCollision(this->board, this->tetromino, this->tetrominoX, this->tetrominoY))
	{
		transpose(this->tetromino);
		reverseRows(this->tetromino);
	}
}

char* getBoardFrame( Game* this )
{
	memcpy(this->frame, this->board, BOARD_LEN);
	fixTetrominoToBoard(this->frame, this->tetromino, this->tetrominoX, this->tetrominoY);
	return (char* )this->frame;
}

void gameOver( Game* this )
{
	destroyGame(this);
	exit(0);
}

void destroyGame( Game* this )
{
	free(this);
}

