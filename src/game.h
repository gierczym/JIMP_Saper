#ifndef _GAME_H_
#define _GAME_H_



typedef struct game {
	int n_mines;
	int init_flag;
	int pos_x;
	int pos_y;
	int board_size_x;
	int board_size_y;
	board_t board_core;
	board_t board_view;
} * game_t;

void generate_mines( game_t game );

game_t initialize_game( int board_size_x, int board_size_y, int n_mines);

int execute_command( game_t game, enum command_t command );

#endif
