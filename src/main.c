#include <stdio.h>
#include <string.h>

#include "userinput.h"
#include "board.h"
#include "game.h"


int main( int argc, char *argv[]) {

	int board_size_x = 9;
	int board_size_y = 9;
	int n_mines = 10;
	enum command_t command; 

	game_t game = initialize_game( board_size_x, board_size_y, n_mines );
	if( argc > 1)
		if( 0 == strcmp( argv[1], "--cheat") )
			game->cheat_flag = 1;

	display_board( game->pos_x, game->pos_y, game->board_core );
	while( ESC != (command = read_command()) ) {
		
		if( 0 == execute_command( game, command ) ) {
			display_board( game->pos_x, game->pos_y, game->board_view );
		} else {
			display_board( game->pos_x, game->pos_y, game->board_core );
			printf( "PRZEGRANA!!!\n" );
			break;
		}
	}

	

	return 0;
}
