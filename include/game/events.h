#ifndef EVENTS_H
#define EVENTS_H

typedef void (* event_handler_fn)( void );
typedef enum
{
	EVENT_STATS_CHANGED,
	EVENT_BOARD_CHANGED,
	EVENT_NEXT_PIECE_CHANGED,
	EVENT_GAME_OVER
} event_t;
typedef struct events_t events_t;

events_t* events_new( void );
void events_free( events_t * );

/* Actions */
void events_register( events_t *, event_t , event_handler_fn );
void events_trigger( events_t *, event_t );

#endif /* EVENTS_H */

