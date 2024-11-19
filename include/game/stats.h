#ifndef STATS_H
#define STATS_H

#define STATS_LINES_PER_LEVEL 10
#define STATS_SCORE_BASE 10
#define STATS_LINE_MULTIPLIER_SINGLE STATS_SCORE_BASE * 1
#define STATS_LINE_MULTIPLIER_DOUBLE STATS_SCORE_BASE * 3
#define STATS_LINE_MULTIPLIER_TRIPLE STATS_SCORE_BASE * 5
#define STATS_LINE_MULTIPLIER_TETRIS STATS_SCORE_BASE * 8

typedef struct stats_t stats_t;

stats_t* stats_create( void );
void stats_destroy( stats_t * );

/* Actions */
void stats_update( stats_t *, int lines );    

/* Getters */
int stats_get_score( stats_t * );
int stats_get_level( stats_t * );
int stats_get_lines_cleared( stats_t * );

#endif // STATS_H
