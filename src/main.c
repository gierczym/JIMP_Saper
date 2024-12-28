#include <stdio.h>

#include "userinput.h"

int main() {

	enum command_t command; 
	while( ESC != (command = read_command()) ) {
		printf("Wczytalem komende: ");
		display_command( command );
	}
	return 0;
}
