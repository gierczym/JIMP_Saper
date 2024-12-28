#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include "conio.h"

#include "userinput.h"

enum command_t read_command(void) {

	char in;
	in = getch(); //non-standard function from library conio.h
	
	if( 'A' == toupper(in) )
		return LEFT;

	if( 'D' == toupper(in) )
		return RIGHT;

	if( 'W' == toupper(in) )
		return UP;

	if( 'S' == toupper(in) )
		return DOWN;
	
	if( 'Q' == toupper(in) )
		return Q;
	
	if( 'E' == toupper(in) )
		return E;
	
	if( 10 == in )
		return ENTER;

	if( 27 == in ) { // ESC or ARROWKEY pressed, we need to distinguish
		fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK); // set terminal to not blocking state
		in = getch();
		usleep(1000); // wait for a milisecond
		fcntl(0, F_SETFL, fcntl(0, F_GETFL) & ~O_NONBLOCK); // set terminal to blocking state again
		if( -1 == in ) // no additional characters, i.e., it is ESC key
			return ESC;
		// there are additional characters, it can be arrowkey if character equals 91
		if( 91 == in ) {
			in = getch();
			if( 68 == in )
				return LEFT;
			if( 67 == in )
				return RIGHT;
			if( 65 == in )
				return UP;
			if( 66 == in )
				return DOWN;
		}

	}

	return MISC; // return MISC, when none of the known commands found
}

void display_command( enum command_t command) {
	switch (command){
		case LEFT:
			printf("LEFT\n");
			break;
		case RIGHT:
			printf("RIGHT\n");
			break;
		case UP:
			printf("UP\n");
			break;
		case DOWN:
			printf("DOWN\n");
			break;
		case Q:
			printf("Q\n");
			break;
		case E:
			printf("E\n");
			break;
		case ENTER:
			printf("ENTER\n");
			break;
		case ESC:
			printf("ESC\n");
			break;
		case MISC:
			printf("MISC\n");
			break;
	}
}
