#include <stdio.h>
#include <stdlib.h>

#include "board.h"

#define SET_COLOR_GREEN	  printf( "\033[0;32m");
#define SET_COLOR_DEFAULT printf( "\033[0m");

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
} fld_disp_t;
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

void print_belt( int n_col, int pos_y, int color_flag ) {

	printf( "   " ); // 3 empty characters
	
	if( color_flag )
		SET_COLOR_GREEN;

	int i;
	for( i = 0; i < n_col; i++ ) {
		if( i == pos_y )
			SET_COLOR_GREEN
		printf( "---" );
		if( !color_flag )
			SET_COLOR_DEFAULT
	}
	printf( "\n" );

	SET_COLOR_DEFAULT;
}

void print_row( int pos_x, int pos_y, board_t board, int color_flag ) {
	
	if( color_flag )
		SET_COLOR_GREEN

	printf( "%2d", pos_x+1 );
	if( 0 == pos_y ) {
		SET_COLOR_GREEN
		printf( "|" );
		if( !color_flag)
			SET_COLOR_DEFAULT
	} else {
		printf( "|" );
	}

	int j;
	for( j = 0; j < board->n_col; j++ ) {
		printf(" %c", board->data[pos_x][j] );
		if( (j == pos_y-1) || (j == pos_y) )
			SET_COLOR_GREEN;
		printf( "|" );
		if( !color_flag )
			SET_COLOR_DEFAULT;
	}

	printf( "\n" );	

	SET_COLOR_DEFAULT
}

void display_board( int pos_x, int pos_y, board_t board ) {
	system( "clear" );
	int i;
	
	printf( "\n" );
	
	//
	// print upper line of coordinates
	if( 0 == pos_y )
		SET_COLOR_GREEN
	printf( "  |" );
	SET_COLOR_DEFAULT
	for( i = 0; i < board->n_col; i++ ) {
		if( i == pos_y-1 ) {
			printf( "%2d", i+1 );
			SET_COLOR_GREEN
			printf( "|" );
			SET_COLOR_DEFAULT
			continue;
		}
		if( i == pos_y ) {
			SET_COLOR_GREEN
			printf( "%2d|", i+1 );
			SET_COLOR_DEFAULT
			continue;
		}
		printf( "%2d|", i+1 );
	}
	printf( "\n" );

	//
	// print upper belt
	if( 0 == pos_x) {
		print_belt( board->n_col, pos_y, 1 );
	} else {
		print_belt( board->n_col, pos_y, 0 );
	}

	//
	// print rows
	for( i = 0; i < board->n_row; i++ ) {
		if( i == pos_x ) {
			print_row( i, pos_y, board, 1 );
		} else {
			print_row( i, pos_y, board, 0 );
		}
		if( (i == pos_x-1) || (i == pos_x) ) {
			print_belt( board->n_col, pos_y, 1 );
		} else {
			print_belt( board->n_col, pos_y, 0 );
		}	
	} 

}










