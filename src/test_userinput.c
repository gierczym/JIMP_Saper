#include <stdio.h>
#include <unistd.h>

#include "userinput.h"
#include "colors.h"


int main( ) {
	
	enum command_t command;
	
	dup2( 1, 0 );

	command = read_command();
	printf( "%c", 'Q' );
	display_command( command );


	printf(" ====================================\n");
	printf(" = TEST MODULU src/userinput.c    ===\n");
	printf(" ====================================\n");

	return 0;
}
