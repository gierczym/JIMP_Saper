#include <stdio.h>
#include <string.h>

#include "userinput.h"
#include "board.h"
#include "game.h"

void clear_buff(){
	while (getchar() != '\n');
}

int main( int argc, char *argv[]) {

	int board_size_x = 0;
	int board_size_y = 0;
	int n_mines = 0;
	int res;
	enum command_t command; 
	
	printf("Witaj w grze saper!\nPodaj poziom trudności\ne - łatwy\nm - średni\nh - trudny\nc - własny\n");
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
				printf("Wybrano własną planszę\nPamiętaj, że jej minimalny rozmiar do 4x4, a ilość min musi być większa od zera i mniejsza o co najmniej 9 od ilości pól\n");
				while(printf("podaj liczbę wierszy ") > 0 && (scanf("%d", &board_size_x) <= 0 || board_size_x < 4)){
					clear_buff();
				}
				while(printf("podaj liczbę kolumn ") > 0 && (scanf("%d", &board_size_y) <= 0 || board_size_y < 4)){
					clear_buff();
				}
				while(printf("podaj liczbę min ") > 0 && (scanf("%d", &n_mines) <= 0 || n_mines < 1 || board_size_x * board_size_y - 9 < n_mines)){
					clear_buff();
				}
				break;
			default:
				printf("podano błędą wartość\npodaj poziom trudności\ne - łatwy\nm - średni\nh - trudny\nc - własny\n");
				difficulty = '\0';
				clear_buff();
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
