#include <stdio.h>


#include "moveshistory.h"
#include "userinput.h"
#include "colors.h"


int main() {

	printf(" ====================================\n");
	printf(" = TEST MODULU src/moveshistory.c ===\n");
	printf(" ====================================\n");

	move_t moves[3];
	moves[0].x = 1; moves[0].y = 2; moves[0].command = Q;
	moves[1].x = 3; moves[1].y = 4; moves[1].command = E;
	moves[2].x = 5; moves[2].y = 6; moves[2].command = Q;

	// testy przypadkow negatywnych
	moves_history_t history = init_moves_history( 2 );
	if( NULL != history ) {
		PRINT_PASS;
		printf( "init_moves_history - udalo sie poprawnie utworzyc strukture danych\n" );
		int flag_added = 0;
		int flag_value = 1;
		int n;
		for( n = 0; n < 3; n++ ) {
			flag_added = flag_added || add_to_moves_history( moves[n].x, moves[n].y, moves[n].command, history );
			if( (moves[n].x != history->moves[n].x ) ||
			    (moves[n].y != history->moves[n].y ) ||
		            (moves[n].command != history->moves[n].command ) )
				flag_value = flag_value && 0;
		}
		if( 0 == flag_added ) {
			PRINT_PASS;
			printf( "add_to_moves_history - udalo sie dodac elementy i przealokowac pamiec\n");
		} else {
			PRINT_FAIL;
			printf( "add_to_moves_history - nie udalo sie dodac elementow i przealokowac pamieci\n");
		}
		if( 1 == flag_value ) {
			PRINT_PASS;
			printf( "add_to_moves_history - poprawne wartosci dodanych elementow\n");
		} else {
			PRINT_FAIL;
			printf( "add_to_moves_history - niepoprawne wartosci dodanych elementow\n");
		}
		free_moves_history( history );
		PRINT_PASS;
		printf( "free_moves_history - udalo sie zwolnic pamiec\n");
	} else {
		PRINT_FAIL;
		printf( "init_moves_history - nie udalo sie utworzyc struktury danych" );
	}

	// testy przypadkow negatywnych
	history = init_moves_history( -1 );
	if( NULL == history ) {
		PRINT_PASS;
		printf( "init_moves_history - poprawnie wykryto nieprawidlowy parametr wejsciowy\n" );
	} else {
		PRINT_FAIL;
		printf( "init_moves_history - nie wykryto nieprawidlowego parametru wejsciowego\n" );
	}
	
	


	return 0;
}
