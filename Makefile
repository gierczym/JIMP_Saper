game: bin/main.o bin/userinput.o bin/board.o bin/gamestruct.o bin/moveshistory.o bin/saves.o
	cc -o bin/game.o bin/main.o bin/userinput.o bin/board.o bin/gamestruct.o bin/moveshistory.o bin/saves.o

bin/main.o: src/main.c
	cc -o bin/main.o -c src/main.c

bin/userinput.o: src/userinput.c
	cc -o bin/userinput.o -c src/userinput.c

bin/board.o: src/board.c
	cc -o bin/board.o -c src/board.c

bin/gamestruct.o: src/game.c
	cc -o bin/gamestruct.o -c src/game.c

bin/moveshistory.o: src/moveshistory.c
	cc -o bin/moveshistory.o -c src/moveshistory.c

bin/saves.o: src/saves.c
	cc -o bin/saves.o -c src/saves.c

clean:
	ls bin/*.o | grep -v game.o | xargs rm

gdb: src/main.c src/userinput.c src/board.c src/game.c
	cc -o bin/game.o src/main.c src/userinput.c src/board.c src/game.c src/moveshistory.c src/saves.c -ggdb
