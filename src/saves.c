#include <stdlib.h>
#include <stdio.h>
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

int save_board(game_t game, FILE *out, int res){
	
    fprintf(out, "%d, %d, %d, %d\n" , game -> board_size_x, game -> board_size_y, game -> n_mines, res);
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
    if(fscanf(in, "%d, %d, %d, %d\n", &game->board_size_x, &game->board_size_y, &game -> n_mines, &game -> expected_res) < 4){
		printf("[!] saves.c/restore_board: błąd wczytywania pierwszej linii pliku, otrzymano mniej niż 4 wartości\n");
		return NULL;
	}
    game->pos_x = game -> board_size_x / 2;
	game->pos_y = game -> board_size_y / 2;

	

    game->board_core = create_board( game -> board_size_x, game -> board_size_y );
	if( NULL == game->board_core ) {
		fprintf( stderr, "[!] saves.c/restore_board: nie udalo sie utworzyc tablicy board_core\n" );
		return NULL;
	}
	game->board_view = create_board( game -> board_size_x, game -> board_size_y );
	if( NULL == game->board_view ) {
		fprintf( stderr, "[!] saves.c/restore_board: nie udalo sie utworzyc tablicy board_view\n" );
		return NULL;
	}
    for(int i = 0; i < game -> n_mines; i++){
        int x, y;
        if(fscanf(in, "%d %d, ", &x, &y) < 2){
			printf("[!] saves.c/restore_board: błąd wczytywania drugiej linii pliku, otrzymano mniej niż 2 koordynaty położenia bomby\n");
			return NULL;
		}
        game -> board_core -> data[x][y] = MINE;
    }
    generate_indicators(game);
    return game;
}

game_t restore_board_autoplay(FILE *in){
	game_t game = restore_board(in);
	if(NULL == game)
		return NULL;
	int x, y;
	char command;	
	int scan_res;
	while(scan_res = fscanf(in, "%d %d %c,", &x, &y, &command)){
		if(scan_res < 3 && scan_res > 0){
			printf("[!] saves.c/restore_board: błąd wczytywania trzeciej linii pliku, otrzymano mniej niż 3 parametry ruchu");
			return NULL;
		}
		else if(scan_res <= 0)
			break;
		game -> pos_x = x;
		game -> pos_y = y;
		switch(execute_command(game, command, 1)){
			case 1:
				game -> actual_res = 1;
				return game;
			case -1:
				game -> actual_res = -1;
				return game;
		}
	}
	game -> actual_res = 0;
	return game;
}                