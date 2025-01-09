#include <stdio.h>


#include "game.h"
#include "userinput.h"
#include "colors.h"


int main() {

	printf(" ====================================\n");
	printf(" = TEST MODULU src/game.c         ===\n");
	printf(" ====================================\n");
	
	int n_row = 5;
	int n_col = 10;
	int n_mines = 5;

	game_t game = initialize_game( n_row, n_col, n_mines );
	if( NULL != game ) {
		PRINT_PASS;
		printf( "initialize_game - udało się poprawnie wygenerować strukturę danych\n" );
		
		generate_mines( game );
		int i;
		int j;
		int mines_ctr;
		board_t board = game->board_core;
		for( i = 0; i < game->board_size_x; i++ )
			for( j = 0; j < game->board_size_y; j++ )
				if( MINE == board->data[i][j] )
					mines_ctr++;
		if( mines_ctr == n_mines ) {
			PRINT_PASS;
			printf( "generate_mines - wygenerowano wlasciwa liczbe min\n" );
		} else {
			PRINT_FAIL;
			printf( "generate_mines - nie wygenerowano wlasciwej liczby min\n" );
		}

		free_game( game );
		PRINT_PASS;
		printf( "free_game - udalo sie zwolnic miejsce na strukture game\n" );

	} else {
		PRINT_FAIL;
		printf( "initialize_game - nie udało się wygenerować struktury danych\n" );
	}

	// testy przypadkow negatywnych
	n_row = -1;
	n_col = 10;
	n_mines = 5;
	game = initialize_game( n_row, n_col, n_mines);
	if( NULL == game ) {
		PRINT_PASS;
		printf( "initialize_game - wlasciwie zwrocono blad gdy liczba wierszy ujemna\n" );
	} else {
		PRINT_FAIL;
		printf( "initialize_game - nie zwrocono bledu gdy liczba wierszy ujemna\n" );
	}
   	n_row = 10;
	n_col = -1;
	n_mines = 5;
	game = initialize_game( n_row, n_col, n_mines);
	if( NULL == game ) {
		PRINT_PASS;
		printf( "initialize_game - wlasciwie zwrocono blad gdy liczba kolumn ujemna\n" );
	} else {
		PRINT_FAIL;
		printf( "initialize_game - nie zwrocono bledu gdy liczba kolumn ujemna\n" );
	}
   	n_row = 5;
	n_col = 10;
	n_mines = -1;
	game = initialize_game( n_row, n_col, n_mines);
	if( NULL == game ) {
		PRINT_PASS;
		printf( "initialize_game - wlasciwie zwrocono blad gdy liczba min ujemna\n" );
	} else {
		PRINT_FAIL;
		printf( "initialize_game - nie zwrocono bledu gdy liczba min ujemna\n" );
	}
   


	return 0;
}
