#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "saves.h"
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

typedef struct moves_history {
	int n;
	int n_curr;
	move_t * moves;
} * moves_history_t;

typedef struct move {
	int x;
	int y;
	enum command_t command;
} move_t;

*/

int save_board(game_t game, FILE *out){
    fprintf(out, "%d,%d\n" , game -> board_size_x, game -> board_size_y);
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

