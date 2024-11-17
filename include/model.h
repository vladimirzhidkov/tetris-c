#ifndef GAME_H 
#define GAME_H

#define LINES_PER_LEVEL 10

/* scoring rules */
#define SCORE_BASE 10 
#define SCORE_LINE_1 SCORE_BASE * 1
#define SCORE_LINE_2 SCORE_BASE * 3
#define SCORE_LINE_3 SCORE_BASE * 5
#define SCORE_LINE_4 SCORE_BASE * 8

/* tetromino */
#define TETROMINO_SIZE 4
#define TETROMINO_LEN TETROMINO_SIZE * TETROMINO_SIZE
typedef char Tetromino[TETROMINO_SIZE][TETROMINO_SIZE];


/* tetromino pool */
#define TETROPOOL_SIZE 7
#define I_SHAPE { \
	{0, 0, 0, 0}, \
	{1, 1, 1, 1}, \
	{0, 0, 0, 0}, \
	{0, 0, 0, 0}  }
#define T_SHAPE { \
	{0, 0, 0, 0}, \
	{1, 1, 1, 0}, \
	{0, 1, 0, 0}, \
	{0, 0, 0, 0}  }
#define S_SHAPE { \
	{0, 0, 0, 0}, \
	{0, 0, 1, 1}, \
	{0, 1, 1, 0}, \
	{0, 0, 0, 0}  }
#define Z_SHAPE { \
	{0, 0, 0, 0}, \
	{0, 1, 1, 0}, \
	{0, 0, 1, 1}, \
	{0, 0, 0, 0}  }
#define O_SHAPE { \
	{0, 0, 0, 0}, \
	{0, 1, 1, 0}, \
	{0, 1, 1, 0}, \
	{0, 0, 0, 0}  }
#define L_SHAPE { \
	{0, 1, 0, 0}, \
	{0, 1, 0, 0}, \
	{0, 1, 1, 0}, \
	{0, 0, 0, 0}  }
#define J_SHAPE { \
	{0, 0, 1, 0}, \
	{0, 0, 1, 0}, \
	{0, 1, 1, 0}, \
	{0, 0, 0, 0}  }

/* board */
#define BOARD_HEIGHT        15
#define BOARD_WIDTH         10
#define BOARD_LEN           BOARD_HEIGHT * BOARD_WIDTH 
typedef char Board[BOARD_HEIGHT][BOARD_WIDTH];

/* game */
typedef struct Game
{
	Tetromino* nextPiece;
	Tetromino* piece;

	int pieceX;
	int pieceY;

	Board staticBoard; // board with tetrominoes fixed in place
	Board activeBoard; // board with tetrominoes that can move

	int score;
	int level;
	int clearedLinesCount;

	/* observers */
	void (* eventStatsChanged)( void );
	void (* eventBoardChanged)( void );
	void (* eventPieceChanged)( void );
	void (* eventGameOver)( void );
} Game;

Game* createGame( void );
void moveLeft( Game* this );
void moveRight( Game* this );
void moveDown( Game* this );
void rotateClockwise( Game* this );
void rotateCounterClockwise( Game* this );
char* getBoard( Game* this );
void destroyGame( Game* this );

#endif // GAME_H