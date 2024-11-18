#include <stdlib.h>
#include "stats.h"

struct stats_t
{
	int score;
	int level;
	int clearedLinesCount;
};

static const int LINES_SCORE_MULTIPLIERS[] =
{
	0,
	STATS_LINE_MULTIPLIER_SINGLE,
	STATS_LINE_MULTIPLIER_DOUBLE,
	STATS_LINE_MULTIPLIER_TRIPLE,
	STATS_LINE_MULTIPLIER_TETRIS
};

stats_t* stats_create( void )
{
	stats_t* this = (stats_t*)malloc( sizeof( stats_t ) );
	this->score = 0;
	this->level = 1;
	this->clearedLinesCount = 0;
	return this;
}

int stats_get_score( stats_t* this )
{
	return this->score;
}

int stats_get_level( stats_t* this )
{
	return this->level;
}

int stats_get_cleared_lines_count( stats_t* this )
{
	return this->clearedLinesCount;
}

void stats_update_cleared_lines_count( stats_t* this, int lines )
{
	this->clearedLinesCount += lines;
	this->score += LINES_SCORE_MULTIPLIERS[lines] * this->level;
    this->level = this->clearedLinesCount / STATS_LINES_PER_LEVEL + 1;
}

void stats_destroy( stats_t* this )
{
	free( this );
}
