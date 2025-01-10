#include <stdio.h>

#include "saves.h"
#include "game.h"
#include "colors.h"


int main() {

	printf(" ====================================\n");
	printf(" = TEST MODULU src/saves.c        ===\n");
	printf(" ====================================\n");


	game_t game = NULL;
    FILE *in = fopen("data/test_1_1", "r");
    game = restore_board_autoplay(in);
    if(game -> actual_res == 1 && game -> correct_moves == 14){
        PRINT_PASS
        printf("restore_board_autoplay - automatyczna gra zakończyła się ze spodziewanym rezultatem - wygrana\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - automatyczna gra nie zakończyła się ze spodziewanym rezultatem - wygrana\n");
    }
    fclose(in);
    free_game(game);

    game = NULL;
    in = fopen("data/test_1_2", "r");
    game = restore_board_autoplay(in);
    if(game -> actual_res == 0 && game -> correct_moves == 8){
        PRINT_PASS
        printf("restore_board_autoplay - automatyczna gra zakończyła się ze spodziewanym rezultatem - przegrana\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - automatyczna gra nie zakończyła się ze spodziewanym rezultatem - przegrana\n");
    }
    fclose(in);
    free_game(game);

    game = NULL;
    in = fopen("data/test_1_3", "r");
    game = restore_board_autoplay(in);
    if(game -> actual_res == 2 && game -> correct_moves == 44){
        PRINT_PASS
        printf("restore_board_autoplay - automatyczna gra zakończyła się ze spodziewanym rezultatem - brak rozstrzygnięcia\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - automatyczna gra nie zakończyła się ze spodziewanym rezultatem - brak rozstrzygnięcia\n");
    }
    fclose(in);
    free_game(game);

    //dane niepoprawne
    game = NULL;
    in = fopen("data/test_2_1", "r");
    printf("\tzwrócony komunikat błędu: ");
    game = restore_board_autoplay(in);
    if(game == NULL){
        PRINT_PASS
        printf("restore_board_autoplay - błędne dane wejściowe: zbyt mało wartości w linijce 1 (brak drugiego wymiaru planszy), zwócono NULL\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - błędne dane wejściowe: zbyt mało wartości w linijce 1 (brak drugiego wymiaru planszy), nie zwócono NULL\n");
    }
    fclose(in);

    
    in = fopen("data/test_2_2", "r");
    printf("\tzwrócony komunikat błędu: ");
    game = restore_board_autoplay(in);
    if(game == NULL){
        PRINT_PASS
        printf("restore_board_autoplay - zbyt dużo wartości w linijce 1 (dodatkowa liczba), zwócono NULL\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - zbyt dużo wartości w linijce 1 (dodatkowa liczba), nie zwócono NULL\n");
    }
    fclose(in);

    in = fopen("data/test_2_3", "r");
    printf("\tzwrócony komunikat błędu: ");
    game = restore_board_autoplay(in);
    if(game == NULL){
        PRINT_PASS
        printf("restore_board_autoplay - zbyt mała ilość min, zwócono NULL\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - zbyt mała ilość min, nie zwócono NULL\n");
    }
    fclose(in);
    
    in = fopen("data/test_2_4", "r");
    printf("\tzwrócony komunikat błędu: ");
    game = restore_board_autoplay(in);
    if(game == NULL){
        PRINT_PASS
        printf("restore_board_autoplay - zbyt duża ilość min, zwócono NULL\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - zbyt duża ilość min, nie zwócono NULL\n");
    }
    fclose(in);

    in = fopen("data/test_2_5", "r");
    printf("\tzwrócony komunikat błędu: ");
    game = restore_board_autoplay(in);
    if(game == NULL){
        PRINT_PASS
        printf("restore_board_autoplay - jeden z ruchów nie ma kompletu informacji, zwócono NULL\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - jeden z ruchów nie ma kompletu informacji, nie zwócono NULL\n");
    }
    fclose(in);

    in = fopen("data/test_2_6", "r");
    printf("\tzwrócony komunikat błędu: ");
    game = restore_board_autoplay(in);
    if(game == NULL){
        PRINT_PASS
        printf("restore_board_autoplay - pusty plik wejściowy, zwócono NULL\n");
    }
    else{
        PRINT_FAIL
        printf("restore_board_autoplay - pusty plik wejściowy, nie zwócono NULL\n");
    }
    fclose(in);

	return 0;
}
