#ifndef _SAVES_H_
#define _SAVES_H_

#include "game.h"
#include "stdio.h"

typedef struct leaderboard_entry{
    char *nickname;
    int points;
} leaderboard_entry_t;

int save_board(game_t game, FILE *out, int res);

int save_moves(moves_history_t history, FILE *out);

game_t restore_board(FILE *in);

game_t restore_board_autoplay(FILE *in);

int append_to_leaderboard(game_t game);
#endif