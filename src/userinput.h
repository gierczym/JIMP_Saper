#ifndef _USERINPUT_H_
#define _USERINPUT_H_


enum command_t {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	E,
	ENTER,
	ESC,
	MISC
};

enum command_t read_command(void);

void display_command( enum command_t in);


#endif
