#ifndef GAME_H 
#define GAME_H

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

/* tetromino */
#define TETROMINO_SIZE 4
#define TETROMINO_LEN TETROMINO_SIZE * TETROMINO_SIZE
typedef char Tetromino[TETROMINO_SIZE][TETROMINO_SIZE];

/* board */
#define BOARD_HEIGHT        15
#define BOARD_WIDTH         10
#define BOARD_LEN           BOARD_HEIGHT * BOARD_WIDTH 
typedef char Board[BOARD_HEIGHT][BOARD_WIDTH];

/* event handlers */
typedef void (* EventHandler)( void );
typedef enum {
	EVENT_STATS_CHANGED,
	EVENT_BOARD_CHANGED,
	EVENT_PIECE_CHANGED,
	EVENT_GAME_OVER
} EventType;

/* game */
typedef struct Game Game;
Game* createGame( void );

/* actions */
void moveLeft( Game* this );
void moveRight( Game* this );
void moveDown( Game* this );
void rotateClockwise( Game* this );
void rotateCounterClockwise( Game* this );
void destroyGame( Game* this );

/* getters */
char* getBoard( Game* this );
char* getNextTetromino( Game* this );

/* event handlers */
void registerEventHandler( Game* this, EventType type, EventHandler handler );

/* stats */
int getGameScore( Game* this );
int getGameLevel( Game* this );
int getGameClearedLinesCount( Game* this );

#endif // GAME_H