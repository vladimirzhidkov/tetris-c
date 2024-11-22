#include <stdlib.h>
#include "stats.h"

struct stats_t
{
	int score;
	int level;
	int lines_cleared;
};

static const int LINES_SCORE_MULTIPLIERS[] =
{
	0,
	STATS_LINE_MULTIPLIER_SINGLE,
	STATS_LINE_MULTIPLIER_DOUBLE,
	STATS_LINE_MULTIPLIER_TRIPLE,
	STATS_LINE_MULTIPLIER_TETRIS
};

stats_t* stats_new( void )
{
	stats_t* this = (stats_t*)malloc( sizeof( stats_t ) );
	*this = (stats_t) {
		.score = 0,
		.level = 1,
		.lines_cleared = 0
	};
	return this;
}

void stats_free( stats_t* this )
{
	free( this );
}



int stats_get_score( stats_t* this )
{
	return this->score;
}

int stats_get_level( stats_t* this )
{
	return this->level;
}

int stats_get_lines_cleared( stats_t* this )
{
	return this->lines_cleared;
}

void stats_update( stats_t* this, int lines )
{
	this->lines_cleared += lines;
	this->score += LINES_SCORE_MULTIPLIERS[lines] * this->level;
    this->level = this->lines_cleared / STATS_LINES_PER_LEVEL + 1;
}

