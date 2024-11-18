#include <stdlib.h>
#include "stats.h"

struct Stats
{
	int score;
	int level;
	int clearedLinesCount;
};

static const int LINES_SCORE_MULTIPLIERS[] =
{
	0,
	LINE_MULTIPLIER_SINGLE,
	LINE_MULTIPLIER_DOUBLE,
	LINE_MULTIPLIER_TRIPLE,
	LINE_MULTIPLIER_TETRIS
};

Stats* createStats( void )
{
	Stats* this = (Stats*)malloc( sizeof( Stats ) );
	this->score = 0;
	this->level = 1;
	this->clearedLinesCount = 0;
	return this;
}

int getScore( Stats* this )
{
	return this->score;
}

int getLevel( Stats* this )
{
	return this->level;
}

int getClearedLinesCount( Stats* this )
{
	return this->clearedLinesCount;
}

void updateClearedLinesCount( Stats* this, int lines )
{
	this->clearedLinesCount += lines;
	this->score += LINES_SCORE_MULTIPLIERS[lines] * this->level;
    this->level = this->clearedLinesCount / LINES_PER_LEVEL + 1;
}

void destroyStats( Stats* this )
{
	free( this );
}
