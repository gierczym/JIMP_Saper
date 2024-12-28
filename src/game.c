#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "userinput.h"
#include "board.h"
#include "game.h"

/*
typedef struct game {
	int n_mines;
	int init_flag;
	int pos_x;
	int pos_y;
	int board_size_x;
	int board_size_y;
	board_t board_core;
	board_t board_view;
} * game_t;
*/

void generate_mines( game_t game ) {

	srand( time(NULL) );
	int pos_x;
	int pos_y;
	int ctr_mines = 0;

	while( ctr_mines < game->n_mines ) {
		pos_x = (double)rand() / RAND_MAX * game->board_size_x;
		pos_y = (double)rand() / RAND_MAX * game->board_size_y;

		//
		// current position is forbidden for mines generation
		if( (pos_x == game->pos_x) && (pos_y == game->pos_y) )
			continue;

		if( ACTIVE == game->board_core->data[pos_x][pos_y] ) {
			game->board_core->data[pos_x][pos_y] = MINE; 
			ctr_mines++;
		}

	}

}


int is_mine( int pos_x, int pos_y, board_t board ) {
	if( (pos_x < 0) || (pos_x >= board->n_row) ||
	    (pos_y < 0) || (pos_y >= board->n_col) )
		return 0;
	if( MINE == board->data[pos_x][pos_y] ) {
		return 1;
	} else {
		return 0;
	}
}

void generate_indicators( game_t game ) {

	int board_size_x = game->board_size_x;
	int board_size_y = game->board_size_y;
	board_t board = game->board_core;
	char **data = board->data;

	int i;
	int j;
	int mine_ctr;
	for( i = 0; i < board_size_x; i++ ) {
		for( j = 0; j < board_size_y; j++ ) {
			mine_ctr = 0;
			if( MINE == data[i][j])
				continue;
			if( is_mine( i-1, j-1, board ) )
				mine_ctr++;
			if( is_mine( i-1, j, board ) )
				mine_ctr++;
			if( is_mine( i-1, j+1, board ) )
				mine_ctr++;
			if( is_mine( i, j-1, board ) )
				mine_ctr++;
			if( is_mine( i, j+1, board ) )
				mine_ctr++;
			if( is_mine( i+1, j-1, board ) )
				mine_ctr++;
			if( is_mine( i+1, j, board ) )
				mine_ctr++;
			if( is_mine( i+1, j+1, board ) )
				mine_ctr++;
			if( 0 == mine_ctr ) {
				data[i][j] = EMPTY;
			} else {
				data[i][j] = '0' + mine_ctr;
			}
		}
	}

}

game_t initialize_game( int board_size_x, int board_size_y, int n_mines) {

	game_t game = malloc( sizeof * game );
	if( NULL == game ) {
		fprintf( stderr, "[!] game.c/initialize_game: nie udalo sie zaalokowac pamieci na game\n" );
		return NULL;
	}
	game->n_mines = n_mines;
	game->init_flag = 0;
	game->pos_x = board_size_x /2;
	game->pos_y = board_size_y /2;
	game->board_size_x = board_size_x;
	game->board_size_y = board_size_y;
	game->board_core = create_board( board_size_x, board_size_y );
	if( NULL == game->board_core ) {
		fprintf( stderr, "[!] game.c/initialize_game: nie udalo sie utworzyc tablicy board_core\n" );
		return NULL;
	}
	game->board_view = create_board( board_size_x, board_size_y );
	if( NULL == game->board_view ) {
		fprintf( stderr, "[!] game.c/initialize_game: nie udalo sie utworzyc tablicy board_view\n" );
		return NULL;
	}

	return game;
}

int execute_command( game_t game, enum command_t command ) {
	if( LEFT == command )
		game->pos_y--;
	if( RIGHT == command )
		game->pos_y++;
	if( UP == command )
		game->pos_x--;
	if( DOWN == command )
		game->pos_x++;
	if( game->pos_x < 0 )
		game->pos_x = 0;
	if( game->pos_x > game->board_size_x-1 )
		game->pos_x = game->board_size_x-1;
	if( game->pos_y < 0 )
		game->pos_y = 0;
	if( game->pos_y > game->board_size_y-1 )
		game->pos_y = game->board_size_y-1;

	if( Q == command )
		if( FLAGGED == game->board_view->data[game->pos_x][game->pos_y] ) {
			game->board_view->data[game->pos_x][game->pos_y] = ACTIVE;
		} else if ( ACTIVE == game->board_view->data[game->pos_x][game->pos_y] ) {
			game->board_view->data[game->pos_x][game->pos_y] = FLAGGED;
		}

	if( E == command ) {
		if( !game->init_flag) {
			game->init_flag = 1;
			generate_mines( game );
			generate_indicators( game );
		}
		if( ACTIVE == game->board_view->data[game->pos_x][game->pos_y] )
			if( MINE == game->board_core->data[game->pos_x][game->pos_y] ) {
				game->board_view->data[game->pos_x][game->pos_y] = 'M';
				return -1;
			}
	}

	return 0;
}
