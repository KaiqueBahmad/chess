#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "../chess/chess.h"

void ui_display_welcome();

void ui_draw_board(chess_board_t board);

void ui_display_turn(int is_white_turn);

void ui_announce_check(int is_white_turn);

void ui_announce_game_end(int is_checkmate, int is_white_turn);

void ui_display_possible_moves(chess_board_t board, chess_move_t moves[], int num_moves);

chess_move_t ui_ask_select_piece(chess_board_t board, int is_white_turn);
chess_move_t ui_ask_select_move(chess_move_t possible_moves[], int num_moves);
#endif
