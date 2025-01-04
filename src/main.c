#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "userinput.h"
#include "board.h"
#include "game.h"
#include "moveshistory.h"
#include "saves.h"

void clear_buff(){
	while (getchar() != '\n');
}

int main( int argc, char *argv[]) {

	int cheat_flag = 0;
	int save_flag = 0;
	int automatic_play_flag = 0;
	int read_board_flag = 0;
	int opt;
	FILE *in = NULL;
	FILE *out = NULL;

	while( (opt = getopt(argc, argv, "ct:r:s:")) != -1 ) {
		switch (opt) {
			case 'c':
				cheat_flag = 1;
				break;
			case 't':
				automatic_play_flag = 1;
				in = fopen( optarg, "r" );
				if( NULL == in ) {
					fprintf( stderr, "[!] main.c: nie udalo sie otworzyc pliku %s\n", optarg );
					return EXIT_FAILURE;
				}
				break;
			case 'r':
				read_board_flag = 1;
				in = fopen( optarg, "r" );
				if( NULL == in ) {
					fprintf( stderr, "[!] main.c: nie udalo sie otworzyc pliku %s\n", optarg );
					return EXIT_FAILURE;
				}
				break;
			case 's':
				save_flag = 1;
				out = fopen( optarg, "w" );
				if( NULL == out ) {
					fprintf( stderr, "[!] main.c: nie udalo sie otworzyc pliku %s\n", optarg );
					return EXIT_FAILURE;
				}
				break;
			default: /* ? */
				fprintf( stderr, "[!] main.c: niewlasciwe wywolanie\n");
				fprintf( stderr, "Uzycie: %s [-c] [-r file_name] [-s file_name] [-t file_name]\n", argv[0] );
				fprintf( stderr, "-c :tryb oszusta, gra uruchamia sie ze wszystkimi minami odkrytymi\n" );
				fprintf( stderr, "-r file_name :plansza czytana jest z pliku file_name\n" );
				fprintf( stderr, "-s file_name :plansza i historia ruchow sa zapisywane po grze w pliku file_name\n" );
				fprintf( stderr, "-t file_name :tryb test - gra automatyczna, plansza i historia ruchow sa czytane z pliku file_name\n" );
				return EXIT_FAILURE;
		}
	}


	int board_size_x = 0;
	int board_size_y = 0;
	int n_mines = 0;
	int res;
	enum command_t command; 
	game_t game;

	if(read_board_flag) {
		system( "clear" );
		game = restore_board(in);
		printf("Pomyślnie załadowano grę, wciśnij dowolny przycisk aby rozpocząć");
	}
	else if (automatic_play_flag){
		char *res_str[] = {"przegrana", "brak rozstrzygnięcia", "wygrana", "niepowodzeniem", "powodzeniem"};
		game = restore_board_autoplay(in);
		printf("Oczekiwany wynik gry: %s, Otrzymany wynik gry: %s, test zakończony %s\n",res_str[game->expected_res + 1], res_str[game->actual_res + 1], res_str[(game -> actual_res == game -> expected_res) + 3]);
		return 0;
	}
	else {
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
					printf("Wybrano własną planszę\nPamiętaj, że jej minimalny rozmiar to 4x4, a ilość min musi być większa od zera i mniejsza o co najmniej 9 od ilości pól\n");
					while(printf("Podaj liczbę wierszy ") > 0 && (scanf("%d", &board_size_x) <= 0 || board_size_x < 4)){
						clear_buff();
					}
					while(printf("Podaj liczbę kolumn ") > 0 && (scanf("%d", &board_size_y) <= 0 || board_size_y < 4)){
						clear_buff();
					}
					while(printf("Podaj liczbę min ") > 0 && (scanf("%d", &n_mines) <= 0 || n_mines < 1 || board_size_x * board_size_y - 9 < n_mines)){
						clear_buff();
					}
					break;
				default:
					printf("Podano błędą wartość\nPodaj poziom trudności\ne - łatwy\nm - średni\nh - trudny\nc - własny\n");
					difficulty = '\0';
					clear_buff();
			}
		}

		game = initialize_game( board_size_x, board_size_y, n_mines );
			if( cheat_flag )
			game->cheat_flag = 1;
		display_board( game->pos_x, game->pos_y, game->board_core, game -> n_mines - game -> flag_ctr);
	}
	
	while( ESC != (command = read_command()) ) {
		res = execute_command( game, command, 0); 
		if( 0 == res ) {
			display_board( game->pos_x, game->pos_y, game->board_view, game -> n_mines - game -> flag_ctr);
		} else if( 1 == res ) {
			display_board( game->pos_x, game->pos_y, game->board_view, 0);
			printf( "WYGRANA!!!\n" );
			break;
		} else {
			display_board( game->pos_x, game->pos_y, game->board_core, game -> n_mines - game -> flag_ctr);
			printf( "PRZEGRANA!!!\n" );
			break;
		}
	}

	int i;
	moves_history_t history = game->moves_history;
	move_t move;
	for( i = 0; i < history->n_curr; i++ ) {
		move = history->moves[i];
		printf( "%3d: (x-%2d;y-%2d) %c\n", i, move.x, move.y, (char)move.command );
	}

	if( save_flag ) {
		save_board(game, out, res);
		save_moves(history, out);
	}

	if( in != NULL )
		fclose(in);
	if( out != NULL )
		fclose(out);

	return 0;
}
