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

int save_board(game_t game, FILE *out){
    fprintf(out, "%d, %d, %d\n" , game -> board_size_x, game -> board_size_y, game -> n_mines);
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
    fscanf(in, "%d, %d, %d", &game->board_size_x, &game->board_size_y, &game -> n_mines);
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
