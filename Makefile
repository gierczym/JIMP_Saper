game: src/main.c src/userinput.c src/board.c src/game.c
	cc -o bin/game.o src/main.c src/userinput.c src/board.c src/game.c

gdb: src/main.c src/userinput.c src/board.c src/game.c
	cc -o bin/game.o src/main.c src/userinput.c src/board.c src/game.c -ggdb
