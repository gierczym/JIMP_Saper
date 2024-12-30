#include <stdio.h>
#include <string.h>

#include "userinput.h"
#include "board.h"
#include "game.h"


int main( int argc, char *argv[]) {

	int board_size_x;
	int board_size_y;
	int n_mines;
	int res;
	enum command_t command; 
	
	printf("Witaj w grze saper!\npodaj poziom trudności\ne - łatwy\nm - średni\nh - trudny\nc - własny\n");
	char difficulty = '\0';
	while(difficulty == '\0'){
		scanf("%c", &difficulty);
		switch (difficulty){
			case 'e':
				board_size_x = 9;
				board_size_y = 9;
				n_mines = 10;
				break;
			case 'm':
				board_size_x = 16;
				board_size_y = 16;
				n_mines = 40;
				break;
			case 'h':
				board_size_x = 16;
				board_size_y = 30;
				n_mines = 99;
				break;
			case 'c':
				//do zrobienia
				return 0;
			default:
				printf("podano błędą wartość\npodaj poziom trudności\ne - łatwy\nm - średni\nh - trudny\nc - własny\n");
				difficulty = '\0';
				while (getchar() != '\n');

		}
	}
	game_t game = initialize_game( board_size_x, board_size_y, n_mines );
	if( argc > 1)
		if( 0 == strcmp( argv[1], "--cheat") )
			game->cheat_flag = 1;

	display_board( game->pos_x, game->pos_y, game->board_core );
	while( ESC != (command = read_command()) ) {
		res = execute_command( game, command ); 
		if( 0 == res ) {
			display_board( game->pos_x, game->pos_y, game->board_view );
		} else if( 1 == res ) {
			display_board( game->pos_x, game->pos_y, game->board_view );
			printf( "WYGRANA!!!\n" );
			break;
		} else {
			display_board( game->pos_x, game->pos_y, game->board_core );
			printf( "PRZEGRANA!!!\n" );
			break;
		}
	}

	

	return 0;
}
