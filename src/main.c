#include <stdio.h>
#include <stdlib.h>
#include "chess/chess.h"
#include "user_interface/user_interface.h"

// Helper function to check if a player has any legal moves
int has_legal_moves(chess_board_t board, int is_white) {
    chess_move_t temp_moves[256];
    chess_board_t temp_board;

    // Scan entire board for pieces of the current player
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            chess_Piece piece = board[i][j];

            // Check if piece belongs to current player
            int is_white_piece = (piece > 0);
            if (is_white_piece != is_white) continue;
            if (piece == EMPTY) continue;

            // Get possible moves for this piece
            int num_moves = chess_possible_moves(board, i, j, temp_moves, 256);

            // Check each move to see if it's legal (doesn't leave king in check)
            for (int m = 0; m < num_moves; m++) {
                // Copy board
                for (int r = 0; r < 8; r++) {
                    for (int c = 0; c < 8; c++) {
                        temp_board[r][c] = board[r][c];
                    }
                }

                // Make the move on temp board
                chess_make_move(temp_board, temp_moves[m]);

                // Check if king is still in check after move
                if (!chess_is_in_check(temp_board, is_white)) {
                    return 1; // Found at least one legal move
                }
            }
        }
    }

    return 0; // No legal moves found
}

// Filter out moves that would leave the king in check
int filter_legal_moves(chess_board_t board, chess_move_t moves[], int num_moves,
                       chess_move_t legal_moves[], int is_white) {
    int legal_count = 0;
    chess_board_t temp_board;

    for (int i = 0; i < num_moves; i++) {
        // Copy the board
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                temp_board[r][c] = board[r][c];
            }
        }

        // Make the move on the temporary board
        chess_make_move(temp_board, moves[i]);

        // Check if the king is in check after the move
        if (!chess_is_in_check(temp_board, is_white)) {
            legal_moves[legal_count++] = moves[i];
        }
    }

    return legal_count;
}

int main() {
    chess_board_t board;
    chess_move_t selected_piece;
    chess_move_t possible_moves[256];
    chess_move_t legal_moves[256];
    int num_possible_moves;
    int num_legal_moves;
    int is_white_turn = 1;
    int game_over = 0;

    // Initialize the chess board
    chess_init_board(board);

    // Display welcome message
    ui_display_welcome();

    // Main game loop
    while (!game_over) {
        // Draw the current board state
        ui_draw_board(board);

        // Check if current player is in check
        if (chess_is_in_check(board, is_white_turn)) {
            ui_announce_check(is_white_turn);

            // Check for checkmate
            if (!has_legal_moves(board, is_white_turn)) {
                ui_announce_game_end(1, is_white_turn); // Checkmate
                game_over = 1;
                continue;
            }
        } else {
            // Check for stalemate (not in check but no legal moves)
            if (!has_legal_moves(board, is_white_turn)) {
                ui_announce_game_end(0, is_white_turn); // Stalemate
                game_over = 1;
                continue;
            }
        }

        // Display whose turn it is
        ui_display_turn(is_white_turn);

        // Loop until player makes a valid move
        int valid_move_made = 0;
        while (!valid_move_made) {
            // Ask player to select a piece
            selected_piece = ui_ask_select_piece(board, is_white_turn);

            // Get all possible moves for the selected piece
            num_possible_moves = chess_possible_moves(board,
                                                      selected_piece.from_row,
                                                      selected_piece.from_col,
                                                      possible_moves,
                                                      256);

            if (num_possible_moves == 0) {
                printf("No possible moves for this piece. Select another piece.\n");
                continue;
            }

            // Filter to only legal moves (ones that don't leave king in check)
            num_legal_moves = filter_legal_moves(board, possible_moves, num_possible_moves,
                                                 legal_moves, is_white_turn);

            if (num_legal_moves == 0) {
                printf("No legal moves for this piece (would leave king in check). Select another piece.\n");
                continue;
            }

            // Display possible moves
            ui_display_possible_moves(board, legal_moves, num_legal_moves);

            // Ask player to select a move
            chess_move_t selected_move = ui_ask_select_move(legal_moves, num_legal_moves);

            // Make the move
            chess_make_move(board, selected_move);

            valid_move_made = 1;
        }

        // Switch turns
        is_white_turn = !is_white_turn;
    }

    // Display final board state
    ui_draw_board(board);

    printf("\nThank you for playing!\n");

    return 0;
}
