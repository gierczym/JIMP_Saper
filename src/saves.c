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
        fprintf( stderr, "[!] saves.c/restore_game: nie udalo sie zaalokowac pamieci na game\n" );
		return NULL;
    }
    game->n_mines = 0;
	game->init_flag = 1;
	game->cheat_flag = 0;
	game->revealed_fields = 0;
	game -> flag_ctr = 0;
	game->moves_history = init_moves_history( 10 );
    fscanf(in, "%d, %d, %d, %d", &game->board_size_x, &game->board_size_y, &game -> n_mines, &game -> expected_res);
    game->pos_x = game -> board_size_x / 2;
	game->pos_y = game -> board_size_y / 2;

	

    game->board_core = create_board( game -> board_size_x, game -> board_size_y );
	if( NULL == game->board_core ) {
		fprintf( stderr, "[!] saves.c/restore_game: nie udalo sie utworzyc tablicy board_core\n" );
		return NULL;
	}
	game->board_view = create_board( game -> board_size_x, game -> board_size_y );
	if( NULL == game->board_view ) {
		fprintf( stderr, "[!] saves.c/restore_game: nie udalo sie utworzyc tablicy board_view\n" );
		return NULL;
	}
    for(int i = 0; i < game -> n_mines; i++){
        int x, y;
        fscanf(in, "%d %d, ", &x, &y);
        game -> board_core -> data[x][y] = MINE;
    }
    generate_indicators(game);
    return game;
}

game_t restore_board_autoplay(FILE *in){
	game_t game = restore_board(in);
	int x, y;
	char command;	
	int known_mines = 0;
	while(fscanf(in, "%d %d %c,", &x, &y, &command) > 0){
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
/*
//printf("%d %d %c,", x, y, command);
		if(command == 'Q')
			continue;
		int new_mine = 1;
		if(game -> board_core -> data[x][y] == MINE){
			return -1;
		}
		int known_x[game -> board_size_x - 1];
		int known_y[game -> board_size_y - 1];
		for(int i = 0; i < known_mines; i++){
			if(known_x[i] == x && known_y[i] == y){
				new_mine = 0;
				break;
			}
		}
		if(new_mine){
			known_x[known_mines] = x;
			known_y[known_mines] = y;
			known_mines++;
			printf("nowa mina: x = %d, y = %d\n", x, y);
		}
		if(known_mines == game -> n_mines){
			//for(int i = 0; i < known_mines; i++){
			//	printf("x = %d, y = %d\n", known_x[i], known_y[i]);
			//}
			return 1;
		}
 */