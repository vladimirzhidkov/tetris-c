#ifndef EVENTS_H
#define EVENTS_H

typedef void (* event_handler_fn)( void );
typedef enum
{
	EVENT_STATS_CHANGED,
	EVENT_BOARD_CHANGED,
	EVENT_PIECE_CHANGED,
	EVENT_GAME_OVER
} event_t;

typedef struct events_t events_t;

events_t* events_create( void );
void events_register( events_t *, event_t type, event_handler_fn handler );
void events_trigger( events_t *, event_t type );
void events_destroy( events_t * );

#endif /* EVENTS_H */

