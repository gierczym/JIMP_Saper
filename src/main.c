#include <stdio.h>

#include "userinput.h"
#include "board.h"



int main() {


	int board_size_x = 16;
	int board_size_y = 30;

	board_t board_view = create_board( board_size_x, board_size_y );

	enum command_t command; 
	int pos_x = board_size_x / 2;
	int pos_y = board_size_y / 2;


	display_board( pos_x, pos_y, board_view );
	while( ESC != (command = read_command()) ) {
		if( LEFT == command )
			pos_y--;
		if( RIGHT == command )
			pos_y++;
		if( UP == command )
			pos_x--;
		if( DOWN == command )
			pos_x++;
		if( pos_x < 0 )
			pos_x = 0;
		if( pos_x > board_size_x-1 )
			pos_x = board_size_x-1;
		if( pos_y < 0 )
			pos_y = 0;
		if( pos_y > board_size_y-1 )
			pos_y = board_size_y-1;

		if( Q == command )
			if( FLAGGED == board_view->data[pos_x][pos_y] ) {
				board_view->data[pos_x][pos_y] = ACTIVE;
			} else if ( ACTIVE == board_view->data[pos_x][pos_y] ) {
				board_view->data[pos_x][pos_y] = FLAGGED;
			}



		display_board( pos_x, pos_y, board_view );
	}

	

	return 0;
}
