#ifndef _GAME_H_
#define _GAME_H_

#include "moveshistory.h"
#include "board.h"

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
	int expected_res;
	int actual_res;
	int read_error;
	char nickname[21];
	int points;
	char difficulty;
	int correct_moves;
	moves_history_t moves_history;
	board_t board_core;
	board_t board_view;
} * game_t;

void generate_mines( game_t game );

game_t initialize_game( int board_size_x, int board_size_y, int n_mines);

int execute_command( game_t game, enum command_t command, int test_flag);

void generate_indicators( game_t game );

void free_game(game_t game);

void free_game_without_move_hst(game_t game);

void calculate_points(game_t game, char difficulty);
#endif
