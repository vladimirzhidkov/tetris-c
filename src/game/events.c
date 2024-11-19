#include <stdlib.h>
#include <string.h>
#include "events.h"

struct events_t
 {
	event_handler_fn on_stats_changed;
	event_handler_fn on_board_changed;
	event_handler_fn on_piece_changed;
	event_handler_fn on_game_over;
}; 

events_t* events_create( void )
{
	events_t* this = (events_t *)malloc(sizeof(events_t));
	memset(this, 0, sizeof(events_t));
	return this;
}

void events_destroy( events_t *this )
{
	free(this);
}


void events_register( events_t *this, event_t type, event_handler_fn handler )
{
    switch (type)
	{
		case EVENT_STATS_CHANGED:
			this->on_stats_changed = handler;
			break;
		case EVENT_BOARD_CHANGED:
			this->on_board_changed = handler;
			break;
		case EVENT_PIECE_CHANGED:
			this->on_piece_changed = handler;
			break;
		case EVENT_GAME_OVER:
			this->on_game_over = handler;
			break;
	}
}

void events_trigger( events_t *this, event_t type )
{
	switch (type)
	{
		case EVENT_STATS_CHANGED:
			this->on_stats_changed();
			break;
		case EVENT_BOARD_CHANGED:
			this->on_board_changed();
			break;
		case EVENT_PIECE_CHANGED:
			this->on_piece_changed();
			break;
		case EVENT_GAME_OVER:
			this->on_game_over();
			break;
	}
}

