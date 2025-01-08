#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "userinput.h"
#include "board.h"
#include "game.h"
#include "moveshistory.h"

/*
typedef struct game {
	int n_mines;
	int init_flag;
	int cheat_flag;
	int revealed_fields;
	int pos_x;
	int pos_y;
	int board_size_x;
	int board_size_y;
	int flag_ctr;
	moves_history_t moves_history;
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
		// current position and adjacent fields ar forbidden
		// (it is needed to increase the user experience)
		if( ((pos_x >=  game->pos_x-1) && (pos_x <= game->pos_x+1)) && ((pos_y >=  game->pos_y-1) && (pos_y <= game->pos_y+1)))
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
	game->cheat_flag = 0;
	game->revealed_fields = 0;
	game->pos_x = board_size_x /2;
	game->pos_y = board_size_y /2;
	game->board_size_x = board_size_x;
	game->board_size_y = board_size_y;
	game -> flag_ctr = 0;
	game->moves_history = init_moves_history( 10 );
	game -> actual_res = 2;
	game -> points = 0;
	game ->correct_moves = -1;
	if( NULL == game->moves_history ) {
		fprintf( stderr, "[!] game.c/initialize_game: nie udalo sie utworzyc moves_history\n" );
		return NULL;
	}
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

int is_active( int pos_x, int pos_y, board_t board ) {
	if( (pos_x < 0) || (pos_x >= board->n_row) ||
	    (pos_y < 0) || (pos_y >= board->n_col) )
		return 0;
	if(  ACTIVE == board->data[pos_x][pos_y] ) {
		return 1;
	} else {
		return 0;
	}
}

void reveal_indicators( int pos_x, int pos_y, game_t game ) {
	if( !is_active( pos_x, pos_y, game->board_view) )
		return;
	
	game->board_view->data[pos_x][pos_y] = game->board_core->data[pos_x][pos_y];
	game->revealed_fields++;
	if( game->board_view->data[pos_x][pos_y] != EMPTY )
		return;
	reveal_indicators( pos_x-1, pos_y-1, game );
	reveal_indicators( pos_x-1, pos_y, game );
	reveal_indicators( pos_x-1, pos_y+1, game );
	reveal_indicators( pos_x, pos_y-1, game );
	reveal_indicators( pos_x, pos_y+1, game );
	reveal_indicators( pos_x+1, pos_y-1, game );
	reveal_indicators( pos_x+1, pos_y, game );
	reveal_indicators( pos_x+1, pos_y+1, game );
}

void reveal_mines( game_t game ) {
	int i;
	int j;
	for( i = 0; i < game->board_size_x; i++ )
		for( j = 0; j < game->board_size_y; j++ )
			if( MINE == game->board_core->data[i][j] )
				game->board_view->data[i][j] = MINE;
}

void reveal_mines_keep_flags( game_t game ) {
	int i;
	int j;
	for( i = 0; i < game->board_size_x; i++ )
		for( j = 0; j < game->board_size_y; j++ )
			if( MINE == game->board_core->data[i][j] && game->board_view->data[i][j] != FLAGGED)
				game->board_view->data[i][j] = MINE;
}

void check_false_flags(game_t game){
	for(int x = 0; x < game->board_size_x; x++ )
		for(int y = 0; y < game->board_size_y; y++ )
			if(MINE != game->board_core->data[x][y] && game->board_view->data[x][y] == FLAGGED)
				game->board_view->data[x][y] = FALSE_FLAGGED;
}
void calculate_points(game_t game, char difficulty){
	int mltp = 0;
	switch(difficulty){
		case 'e':
			mltp = 1;
			break;
		case 'm':
			mltp = 2;
			break;
		case 'h':
			mltp = 3;
			break;
		case 'c':
			mltp = 0;
			break;
		default:
			printf("[!] game.c/calculate_points: błąd odczytu poziomu trudności");
	}
	game -> points = mltp * game->revealed_fields;
}

int execute_command( game_t game, enum command_t command, int test_flag) {
	if(test_flag == 0){
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

		if( (Q == command) || (E == command) )
			add_to_moves_history( game->pos_x, game->pos_y, command,
					game->moves_history );
	}
	if( Q == command )
		if( FLAGGED == game->board_view->data[game->pos_x][game->pos_y] ) {
			game->board_view->data[game->pos_x][game->pos_y] = ACTIVE;
			game -> flag_ctr--;
		} else if ( ACTIVE == game->board_view->data[game->pos_x][game->pos_y] ) {
			game->board_view->data[game->pos_x][game->pos_y] = FLAGGED;
			game -> flag_ctr++;
		}

	if( E == command ) {
		if( !game->init_flag) {
			game->init_flag = 1;
			generate_mines( game );
			generate_indicators( game );
			if( game->cheat_flag )
				reveal_mines( game );
		}
		if( ACTIVE == game->board_view->data[game->pos_x][game->pos_y] ){
			if( MINE == game->board_core->data[game->pos_x][game->pos_y] ) {
				game->board_view->data[game->pos_x][game->pos_y] = 'M';
				reveal_mines_keep_flags(game);
				check_false_flags(game);
				game->actual_res = 0;
				return -1;
			}
			reveal_indicators( game->pos_x, game->pos_y, game );
			if( game->revealed_fields == 
			    game->board_size_x * game->board_size_y - game->n_mines ){
				reveal_mines_keep_flags(game);
				game->actual_res = 1;
				game->correct_moves++;
				return 1;
			
			}	
			game->correct_moves++;
		}
	}
	return 0;
}

void free_game(game_t game){
	free_moves_history(game->moves_history);
	free_game_without_move_hst(game);

}
void free_game_without_move_hst(game_t game){
	free(game->board_core->data);
	free(game->board_view->data);
	free(game->board_core);
	free(game->board_view);
	free(game);
}

