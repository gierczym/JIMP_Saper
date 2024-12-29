#include <stdio.h>
#include <stdlib.h>

#include "board.h"

#define SET_COLOR_GREEN	  printf( "\033[0;32m")
#define SET_COLOR_BRIGHTGREEN	  printf( "\033[0;92m")
#define SET_COLOR_RED	  printf( "\033[0;31m")
#define SET_COLOR_BRIGHTRED	  printf( "\033[0;91m")
#define SET_COLOR_BLUE	  printf( "\033[0;34m")
#define SET_COLOR_BRIGHTBLUE	  printf( "\033[0;94m")
#define SET_COLOR_MAGENTA	  printf( "\033[0;35m")
#define SET_COLOR_CYAN	  printf( "\033[0;36m")
#define SET_COLOR_YELLOW	  printf( "\033[0;33m")
#define SET_COLOR_GREY	  printf( "\033[0;90m")
#define SET_COLOR_DEFAULT printf( "\033[0m")

/*
typedef struct brd {
	int n_row;
	int n_col;
	char **data;
} * board_t;

typedef enum fld_disp {
	ACTIVE = ' ',
	ONE = '1',
	TWO = '2',
	THREE = '3',
	FOUR = '4',
	FIVE = '5',
	SIX = '6',
	SEVEN = '7',
	EIGHT = '8',
	FLAGGED = 'F',
	EMPTY = 'X',
	MINE = 'o'
} fld_t;
*/

board_t create_board( int n_row, int n_col ) {
	
	board_t board = malloc( sizeof * board );
	if( NULL == board ) {
		fprintf( stderr, "[!] board.c/create_board: nie udalo sie zaalokowac pamieci na typ board_t\n" );
		return NULL;
	}
	board->n_row = n_row;
	board->n_col = n_col;
	board->data = malloc( n_row * sizeof * board->data );
	if( NULL == board->data ) {
		fprintf( stderr, "[!] board.c/create_board: nie udalo sie zaalokowac pamieci na board->data\n" );
		return NULL;
	}
	int i;
	int j;
	for( i = 0; i < n_row; i++ ){
		board->data[i] = malloc( n_col * sizeof(char) );
		if( NULL == board->data[i] ) {
			fprintf( stderr, "[!] board.c/create_board: nie udalo sie zaalokowac pamieci na %d-ty wiersz planszy\n", i+1 );
			return NULL;
		}
		for( j = 0; j < n_col; j++ ) {
			board->data[i][j] = ACTIVE;
		}
	}

	return board;
}

void print_belt( int n_col, int color_flag ) {
	
	if( color_flag )
		SET_COLOR_GREEN;

	printf( "---" );

	int i;
	for( i = 0; i < n_col; i++ ) {
		printf( "---" );
	}
	printf( "\n" );

	SET_COLOR_DEFAULT;
}

int is_edge_horizontal( int pos_x, int pos_y, board_t board ) {

	if( pos_x == board->n_row-1 )
		return 0;

	if( (board->data[pos_x][pos_y] >= '1') && (board->data[pos_x][pos_y] <= '8')
		       && ((ACTIVE == board->data[pos_x+1][pos_y]) || (FLAGGED == board->data[pos_x+1][pos_y])) )
		return 1;
	if( (board->data[pos_x+1][pos_y] >= '1') && (board->data[pos_x+1][pos_y] <= '8')
		       && ((ACTIVE == board->data[pos_x][pos_y]) || (FLAGGED == board->data[pos_x][pos_y])) )
		return 1;
	return 0;
}

void print_belt_partial( int n_col, int pos_x, int pos_y, int color_flag, board_t board ) {

	if( color_flag ) {
		SET_COLOR_GREEN;
	}
	printf( "--" );
	SET_COLOR_DEFAULT;
	
	if( (0 == pos_y) || color_flag ) {
		SET_COLOR_GREEN;
	}
	printf( "|" );
	SET_COLOR_DEFAULT;
	
	int i;
	for( i = 0; i < n_col-1; i++ ) {
		if( (i == pos_y) && color_flag  ) {
			SET_COLOR_GREEN;
			printf( "-- " );
			SET_COLOR_DEFAULT;
			continue;
		}
		if( is_edge_horizontal( pos_x, i, board) ) {
			printf( "-- " );
		} else {
			printf( "   " );
		}
		
	}

	if( (pos_y == n_col-1) && color_flag ) {
		SET_COLOR_GREEN;
		printf( "--|\n" );
		SET_COLOR_DEFAULT;
		return;
	}
	if( is_edge_horizontal( pos_x, i, board) ) {
		printf( "--" );
	} else {
		printf( "  " );
	}
	if( pos_y == n_col-1 )
		SET_COLOR_GREEN;
	printf( "|\n" );
	SET_COLOR_DEFAULT;

}


void print_character( fld_t fld ) {
	switch (fld) {
		case EMPTY:
			SET_COLOR_GREY;
			break;
		case ONE:
			SET_COLOR_BLUE;
			break;
		case TWO:
			SET_COLOR_BRIGHTGREEN;
			break;
		case THREE:
			SET_COLOR_RED;
			break;
		case FOUR:
			SET_COLOR_BRIGHTBLUE;
			break;
		case FIVE:
			SET_COLOR_CYAN;
			break;
		case SIX:
			SET_COLOR_MAGENTA;
			break;
		case SEVEN:
			SET_COLOR_YELLOW;
			break;
		case EIGHT:
			SET_COLOR_DEFAULT;
			break;
		case FLAGGED:
			SET_COLOR_RED;
			break;
	}
	printf( " %c", fld );
	SET_COLOR_DEFAULT;
}

int is_edge_vertical( int pos_x, int pos_y, board_t board ) {

	if( pos_y == board->n_col-1 )
		return 0;

	if( (board->data[pos_x][pos_y] >= '1') && (board->data[pos_x][pos_y] <= '8')
		       && ((ACTIVE == board->data[pos_x][pos_y+1]) || (FLAGGED == board->data[pos_x][pos_y+1])) )
		return 1;
	if( (board->data[pos_x][pos_y+1] >= '1') && (board->data[pos_x][pos_y+1] <= '8')
		       && ((ACTIVE == board->data[pos_x][pos_y]) || (FLAGGED == board->data[pos_x][pos_y])) )
		return 1;
	return 0;
}

void print_row( int pos_x, int pos_y, board_t board, int color_flag ) {
	
	if( color_flag )
		SET_COLOR_GREEN;
	printf( "%2d", pos_x+1 );
	SET_COLOR_DEFAULT;
	if( (0 == pos_y) || color_flag ) {
		SET_COLOR_GREEN;
	}
	printf( "|" );
	SET_COLOR_DEFAULT;
	

	int j;
	for( j = 0; j < board->n_col; j++ ) {
		if( (j == pos_y) && color_flag ) {
			SET_COLOR_GREEN;
			printf(" %c|", board->data[pos_x][j] );
			SET_COLOR_DEFAULT;
			continue;
		}
		print_character( board->data[pos_x][j] );
		if( ((j == pos_y-1) && color_flag) ) {
			SET_COLOR_GREEN;
			printf( "|" );
			SET_COLOR_DEFAULT;
			continue;
		}
		if( is_edge_vertical(pos_x, j, board) || (j == board->n_col-1) ) {
			if( pos_y == board->n_col-1 )
				SET_COLOR_GREEN;
			printf( "|" );
		} else {
			printf( " " );
		}
	}
	
	printf( "\n" );	

	SET_COLOR_DEFAULT;
}

void display_board( int pos_x, int pos_y, board_t board ) {
	system( "clear" );
	int i;
	
	printf( "\n" );
	
	//
	// print upper line of coordinates
	if( 0 == pos_y )
		SET_COLOR_GREEN;
	printf( "  |" );
	SET_COLOR_DEFAULT;
	for( i = 0; i < board->n_col; i++ ) {
		if( i == pos_y-1 ) {
			printf( "%2d", i+1 );
			SET_COLOR_GREEN;
			printf( "|" );
			SET_COLOR_DEFAULT;
			continue;
		}
		if( i == pos_y ) {
			SET_COLOR_GREEN;
			printf( "%2d|", i+1 );
			SET_COLOR_DEFAULT;
			continue;
		}
		printf( "%2d|", i+1 );
	}
	printf( "\n" );

	//
	// print upper belt
	if( 0 == pos_x) {
		print_belt( board->n_col, 1 );
	} else {
		print_belt( board->n_col, 0 );
	}

	//
	// print rows
	for( i = 0; i < board->n_row; i++ ) {
		if( i == pos_x ) {
			print_row( i, pos_y, board, 1 );
		} else {
			print_row( i, pos_y, board, 0 );
		}
		if( i == board->n_row-1 )
			if( i == pos_x) {
				print_belt( board->n_col, 1 );
				break;
			} else {
				print_belt( board->n_col, 0 );
				break;
			}
		if( (i == pos_x-1) || (i == pos_x) ) { 
			print_belt_partial( board->n_col, i, pos_y, 1, board );
		} else {
			print_belt_partial( board->n_col, i, pos_y, 0, board );
		}
	}

}










