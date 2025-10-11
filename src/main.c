#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chess/chess.h"

void init_board(board_t b) {
    // Clear board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            b[i][j] = EMPTY;
        }
    }

    // White pieces (row 0)
    b[0][0] = WHITE_ROOK;
    b[0][1] = WHITE_KNIGHT;
    b[0][2] = WHITE_BISHOP;
    b[0][3] = WHITE_QUEEN;
    b[0][4] = WHITE_KING;
    b[0][5] = WHITE_BISHOP;
    b[0][6] = WHITE_KNIGHT;
    b[0][7] = WHITE_ROOK;

    // White pawns (row 1)
    for (int j = 0; j < 8; j++) {
        b[1][j] = WHITE_PAWN;
    }

    // Black pieces (row 7)
    b[7][0] = BLACK_ROOK;
    b[7][1] = BLACK_KNIGHT;
    b[7][2] = BLACK_BISHOP;
    b[7][3] = BLACK_QUEEN;
    b[7][4] = BLACK_KING;
    b[7][5] = BLACK_BISHOP;
    b[7][6] = BLACK_KNIGHT;
    b[7][7] = BLACK_ROOK;

    // Black pawns (row 6)
    for (int j = 0; j < 8; j++) {
        b[6][j] = BLACK_PAWN;
    }
}

void print_moves(move_t moves[], int count) {
    const char* cols = "abcdefgh";

    printf("Possible moves (%d):\n", count);
    for (int i = 0; i < count; i++) {
        printf("%2d. %c%d -> %c%d",
            i + 1,
            cols[moves[i].from_col], moves[i].from_row + 1,
            cols[moves[i].to_col], moves[i].to_row + 1);

        if (moves[i].captured != 0) {
            printf(" (capture)");
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    board_t board;
    move_t moves[128];
    int move_count;

    printf("=== Ultralight Chess Library Demo ===\n");

    // Initialize board
    init_board(board);
    print_board(board);

    // Test: show possible moves for white knight at b1
    printf("Testing piece at b1 (White Knight):\n");
    move_count = possible_moves(board, 0, 1, moves, 128);
    print_moves(moves, move_count);

    // Test: show possible moves for white pawn at e2
    printf("Testing piece at e2 (White Pawn):\n");
    move_count = possible_moves(board, 1, 4, moves, 128);
    print_moves(moves, move_count);

    // Test check detection
    printf("Is white in check? %s\n\n", is_at_check(board) ? "YES" : "NO");

    // Move white pawn e2-e4
    printf("Moving white pawn e2 -> e4\n");
    board[3][4] = board[1][4];
    board[1][4] = EMPTY;
    print_board(board);

    // Show moves for the moved pawn
    printf("Testing piece at e4 (White Pawn):\n");
    move_count = possible_moves(board, 3, 4, moves, 128);
    print_moves(moves, move_count);

    printf("Demo complete!\n");

    return 0;
}
