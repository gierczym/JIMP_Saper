#include <stdio.h>
#include <stdlib.h>

#include "moveshistory.h"

moves_history_t init_moves_history( int n ) {
	if( n < 1 )
		return NULL;

	moves_history_t moves_history = malloc( sizeof * moves_history );
	if( NULL == moves_history ) {
		#ifndef TEST
			fprintf( stderr, "[!] moveshistory.c/init_moves_history: nie udalo sie zanicjowac pamieci na moves_history\n" );
		#endif
		return NULL;
	}
	moves_history->n = n;
	moves_history->n_curr = 0;
	moves_history->moves = malloc( n * sizeof * moves_history->moves );
	if( NULL == moves_history->moves ) {
		#ifndef TEST
			fprintf( stderr, "[!] moveshistory.c/init_moves_history: nie udalo sie zanicjowac pamieci na moves\n" );
		#endif
		return NULL;
	}

	return moves_history;
}

int add_to_moves_history( int x, int y, enum command_t command, moves_history_t moves_history) {
	if( moves_history->n_curr == moves_history->n ) {
		move_t * tmp = realloc( moves_history->moves, 2 * moves_history->n * sizeof * tmp );
		if( NULL == tmp ) {
			#ifndef TEST
				fprintf( stderr, "[!] moveshistory.c/add_to_moves_history: nie udalo sie przealokowac pamieci na moves\n" );
			#endif
			return EXIT_FAILURE;
		}
		moves_history->moves = tmp;
		moves_history->n = 2 * moves_history->n;
	}
	move_t * move = &moves_history->moves[moves_history->n_curr];
	move->x = x;
	move->y = y;
	move->command = command;
	moves_history->n_curr++;

	return 0;
}

void free_moves_history( moves_history_t moves_history ) {
	free( moves_history->moves );
	free( moves_history );
}
