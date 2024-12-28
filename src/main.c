#include <stdio.h>

#include "userinput.h"

int main() {

	enum command_t command; 

	printf( "\033[0;32m"); // set color to green
	while( ESC != (command = read_command()) ) {
		printf("Wczytalem komende: ");
		display_command( command );
	}
	
	printf( "\033[0m"); // reset color to default

	return 0;
}
