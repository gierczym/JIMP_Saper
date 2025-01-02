game: src/main.c src/userinput.c src/board.c src/game.c src/moveshistory.c
	cc -o bin/game.o src/main.c src/userinput.c src/board.c src/game.c src/moveshistory.c

gdb: src/main.c src/userinput.c src/board.c src/game.c
	cc -o bin/game.o src/main.c src/userinput.c src/board.c src/game.c -ggdb
