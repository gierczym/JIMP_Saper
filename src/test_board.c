#include <stdio.h>


#include "board.h"
#include "colors.h"


int main() {

    int n_row = 5;
    int n_col = 10;

    board_t board = create_board(n_row, n_col);

    printf(" ====================================\n");
    printf(" = TEST MODULU src/board.c        ===\n");
    printf(" ====================================\n");

    if( NULL != board ) { // testy przypadkow pozytywnych
	    PRINT_PASS;
	    printf( "create board - udalo sie utworzyc strukture danych\n" );
	    int i;
	    int j;
	    int flag = 1;
	    for( i = 0; i < n_row; i++ )
	    	for( j = 0; j < n_col; j++ )
			if( board->data[i][j] != ACTIVE )
				flag = flag && 0;
	    if( 1 == flag ) {
		    PRINT_PASS;
		    printf( "create board - plansza zainicjalizowana z prawidlowym rozmiarem oraz prawidlowymi wartosciami\n" );
	    } else {
		    PRINT_FAIL;
		    printf( "create board - plansza niewlasciwie zainicjalizowana\n" );
	    }

	    free_board( board );
	    PRINT_PASS;
	    printf( "free_board - udalo sie zwolnic pamiec\n" );
    } else {
	    PRINT_FAIL;
	    printf( "create board - nie udalo sie utworzyc planszy\n" );

    }

    // testy przypadkow negatywnych
    n_row = -1;
    n_col = 10;
    board = create_board(n_row, n_col);
    if( NULL == board ) {
	    PRINT_PASS;
	    printf( "create_board - wlasciwie zwrocono blad gdy liczba wierszy ujemna\n" );
    } else {
	    PRINT_FAIL;
	    printf( "create_board - nie zwrocono bledu gdy liczba wierszy ujemna\n" );
    }
    n_row = 5;
    n_col = -1;
    board_t board2 = create_board(n_row, n_col);
    if( NULL == board2 ) {
	    PRINT_PASS;
	    printf( "create_board - wlasciwie zwrocono blad gdy liczba kolumn ujemna\n" );
    } else {
	    PRINT_FAIL;
	    printf( "create_board - nie zwrocono bledu gdy liczba kolumn ujemna\n" );
    }

    return 0;
}
