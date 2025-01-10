#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "saves.h"
#include "moveshistory.h"
#include "game.h"

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

typedef struct brd {
	int n_row;
	int n_col;
	char **data;
} * board_t;


*/

int save_board(game_t game, FILE *out, int res, char diff){
    fprintf(out, "%d, %d, %d, %d, %c\n" , game -> board_size_x, game -> board_size_y, game -> n_mines, res, diff);
    for(int x = 0; x < game -> board_size_x; x++){
        for(int y = 0; y < game -> board_size_y; y++){
            if(game -> board_core -> data[x][y] == MINE){
                fprintf(out, "%d %d, ", x, y);
            }
        }
    }
    fprintf(out, "\n");
    return 0; 
}
int save_moves(moves_history_t history, FILE *out){
    for(int i = 0; i < history -> n_curr; i++){
        move_t move = history -> moves[i];
        fprintf(out, "%d %d %c, ", move.x, move.y, move.command);
    }
    return 0;
}
game_t restore_board(FILE *in){
    game_t game = malloc(sizeof *game);
    if(NULL == game){
        fprintf( stderr, "[!] saves.c/restore_board: nie udalo sie zaalokowac pamieci na game\n" );
		return NULL;
    }
    game->n_mines = 0;
	game->init_flag = 1;
	game->cheat_flag = 0;
	game->revealed_fields = 0;
	game -> flag_ctr = 0;
	game->moves_history = init_moves_history( 10 );
	game -> read_error = 0;
	game->correct_moves = -1;

	char separator;
	int mines_read = 0;
    if(fscanf(in, "%d, %d, %d, %d, %c%c", &game->board_size_x, &game->board_size_y, &game -> n_mines, &game -> expected_res, &game -> difficulty, &separator) != 6 || separator != '\n'){
		free(game);
		printf("[!] saves.c/restore_board: błąd wczytywania pierwszej linii pliku, otrzymano inną ilość wartości niż 5\n");
		return NULL;
	}

    game->pos_x = game -> board_size_x / 2;
	game->pos_y = game -> board_size_y / 2;	

    game->board_core = create_board( game -> board_size_x, game -> board_size_y );
	if( NULL == game->board_core ) {
		fprintf( stderr, "[!] saves.c/restore_board: nie udalo sie utworzyc tablicy board_core\n" );
		free(game);
		return NULL;
	}
	game->board_view = create_board( game -> board_size_x, game -> board_size_y );
	if( NULL == game->board_view ) {
		fprintf( stderr, "[!] saves.c/restore_board: nie udalo sie utworzyc tablicy board_view\n" );
		game->read_error = 1;
		return game;
	}
	char potential_separator;
    while(1){
        int x, y;
        if(fscanf(in, "%d %d%c%c", &x, &y, &separator, &potential_separator) < 3){
			printf("[!] saves.c/restore_board: błąd wczytywania drugiej linii pliku, otrzymano mniej niż 2 koordynaty położenia bomby\n");
			game -> read_error = 1;
			return game;
		}
		
		if(separator != ','){
			if(mines_read != game->n_mines){
				printf("[!] saves.c/restore_board: błąd wczytywania drugiej linii pliku, wczytano za małą ilość min, oczekiwano: %d, przeczytano: %d\n", game->n_mines, mines_read);
				game->read_error = 1;
				return game;
			}
		}
		mines_read++;
		if(mines_read == game->n_mines+1){
			if(potential_separator != '\n'){
				break;
			}
			else{
				printf("[!] saves.c/restore_board: błąd wczytywania drugiej linii pliku, wczytano za dużą ilość min, oczekiwano: %d, przeczytano: %d\n", game->n_mines, mines_read);
				game->read_error = 1;
				return game;
			}
		}
        game -> board_core -> data[x][y] = MINE;
    }
	
    generate_indicators(game);
	fseek(in, sizeof(char)*(-5), SEEK_CUR);
    return game;
}

game_t restore_board_autoplay(FILE *in){
	game_t game = restore_board(in);
	if(NULL == game){
		return NULL;
	}
	else if(game->read_error == 1){
		free_game_without_move_hst(game);
		return NULL;
	}
	int x, y;
	char command;	
	int scan_res;
	while(scan_res = fscanf(in, "%d %d %c,", &x, &y, &command)){
		//printf("x = %d, y = %d, com = %c", x, y, command);
		if(scan_res <= 0)
			break;
		else if(scan_res != 3){
			printf("[!] saves.c/restore_board: błąd wczytywania trzeciej linii pliku, nie otrzymano 3 parametrów ruchu\n");
			free_game_without_move_hst(game);
			game->read_error = 1;
			return NULL;
		}
		
		game -> pos_x = x;
		game -> pos_y = y;
		switch(execute_command(game, command, 1)){
			case 1:
				game -> actual_res = 1;
				return game;
			case -1:
				game -> actual_res = 0;
				return game;
		}
	}
	game -> actual_res = 2;
	return game;
}

int append_to_leaderboard(game_t game){
	int dyn_vect_size = 2;
	leaderboard_entry_t **leaderboard = malloc(sizeof (leaderboard_entry_t) * (dyn_vect_size+1));
	if(NULL == leaderboard){
		printf("[!] saves.c/append_to_leaderboard: alokacja pamięci na tablicę wyników nie powiodła się");
		return -1;
	}
	FILE *lb = fopen("data/leaderboard.dat", "r");
	int n = 0;
	while(1){
		if(n == dyn_vect_size){
			if(NULL == (leaderboard = realloc(leaderboard, sizeof(leaderboard_entry_t)*dyn_vect_size*2))){
				printf("[!] saves.c/append_to_leaderboard: realokacja pamięci na tablicę wyników nie powiodła się");
				return -2;
			}
			dyn_vect_size *= 2;
		}
		if(NULL == (leaderboard[n] = malloc(sizeof (leaderboard_entry_t)))){
			printf("[!] saves.c/append_to_leaderboard: alokacja pamięci na wpis w tablicy wyników nie powiodła się");
			return -1;
		}
		if(NULL == (leaderboard[n] -> nickname = malloc(sizeof (char) * 21))){
			printf("[!] saves.c/append_to_leaderboard: alokacja pamięci na pseudonim we wpisie w tablicy wyników nie powiodła się");
			return -1;
		}
		if(fscanf(lb, "%s %d\n", leaderboard[n] -> nickname, &leaderboard[n] -> points) != 2){
			break;
		}
		n++;
	}
	fclose(lb);	
	int to_append = 1;
	for(int i = 0; i < n; i++){
		if(game->points > leaderboard[i] -> points){
			int j;
			for(j = n; j>i; j--){
				strcpy(leaderboard[j]->nickname, leaderboard[j-1]->nickname);
				leaderboard[j]->points = leaderboard[j-1]->points;		
			}
			strcpy(leaderboard[j]->nickname, game->nickname);
			leaderboard[j]->points = game->points;
			to_append = 0;
			n++;
			break;
		}
	}
	if(to_append == 1){
		strcpy(leaderboard[n] -> nickname, game -> nickname);
		leaderboard[n] -> points = game -> points;
		n++;
	}
	FILE *lb_save = fopen("data/leaderboard.dat", "w");
	for(int i = 0; i < n; i++){
		fprintf(lb_save, "%s %d\n", leaderboard[i] -> nickname, leaderboard[i] -> points);
	}
	fclose(lb_save);
	//
	//print top 5 players
	int tmp_print = n;
	if(tmp_print > 5)
		tmp_print = 5;
	printf("\n             Aktualnie najlepszych pięciu graczy:\n");
	printf(" =============================================================\n");
	printf(" |Miejsce:   |Pseudonim:                     |Liczba punktów:|\n");
	printf(" =============================================================\n");
	for(int i = 0; i < tmp_print; i++){
		printf(" |%11d| %30s|%15d|\n", i+1, leaderboard[i] -> nickname, leaderboard[i] -> points);
		printf(" |-----------------------------------------------------------|\n");
	}
	//
	//free memory
	for(int i = 0; i < n; i++){
		free(leaderboard[i]->nickname);
		free(leaderboard[i]);
	}
	free(leaderboard);
}
