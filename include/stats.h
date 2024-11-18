#ifndef STATS_H
#define STATS_H

#define LINES_PER_LEVEL 10

#define SCORE_BASE 10
#define LINE_MULTIPLIER_SINGLE SCORE_BASE * 1
#define LINE_MULTIPLIER_DOUBLE SCORE_BASE * 3
#define LINE_MULTIPLIER_TRIPLE SCORE_BASE * 5
#define LINE_MULTIPLIER_TETRIS SCORE_BASE * 8

typedef struct Stats Stats;

Stats* createStats( void );
int getScore( Stats * );
int getLevel( Stats * );
int getClearedLinesCount( Stats * );
void updateClearedLinesCount( Stats *, int lines );    
void destroyStats( Stats* );

#endif // STATS_H
