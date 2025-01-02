#ifndef _MOVESHOSTORY_H_
#define _MOVESHOSTORY_H_

#include "userinput.h"

typedef struct move {
	int x;
	int y;
	enum command_t command;
} move_t;

typedef struct moves_history {
	int n;
	int n_curr;
	move_t * moves;
} * moves_history_t;

moves_history_t init_moves_history( int n );

int add_to_moves_history( int x, int y, enum command_t command, moves_history_t moves_history);

void free_moves_history( moves_history_t moves_history );

#endif
