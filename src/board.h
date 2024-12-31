#ifndef _BOARD_H_
#define _BOARD_H_

typedef struct brd {
	int n_row;
	int n_col;
	char **data;
} * board_t;

typedef enum fld {
	ACTIVE = ' ',
	ONE = '1',
	TWO = '2',
	THREE = '3',
	FOUR = '4',
	FIVE = '5',
	SIX = '6',
	SEVEN = '7',
	EIGHT = '8',
	FLAGGED = '@',
	EMPTY = 'X',
	MINE = 'o'
} fld_t;

board_t create_board( int n_row, int n_col );

void display_board( int pos_x, int pos_y, board_t board, int mines_left);

#endif
