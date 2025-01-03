#ifndef _SAVES_H_
#define _SAVES_H_

#include "game.h"
#include "stdio.h"

int save_board(game_t game, FILE *out);

int save_moves(moves_history_t history, FILE *out);

game_t restore_board(FILE *in);

#endif