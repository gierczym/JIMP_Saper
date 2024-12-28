#include <stdio.h>

#include "userinput.h"
#include "board.h"
#include "game.h"


int main() {


	int board_size_x = 9;
	int board_size_y = 9;
	int n_mines = 10;

	game_t game = initialize_game( board_size_x, board_size_y, n_mines );

	enum command_t command; 

	display_board( game->pos_x, game->pos_y, game->board_core );
	while( ESC != (command = read_command()) ) {
		
		take_command( game, command );

		display_board( game->pos_x, game->pos_y, game->board_core );
	}

	

	return 0;
}
