#ifndef CHESS_H
#define CHESS_H

typedef signed char board_t[8][8];

typedef enum {
    BLACK_KING = -1,
    BLACK_QUEEN = -2,
    BLACK_ROOK = -3,
    BLACK_BISHOP = -4,
    BLACK_KNIGHT = -5,
    BLACK_PAWN = -6,
    BLACK_EN_PASSANT_PAWN = -7,
    BLACK_MOVED_KING = -8,
    BLACK_MOVED_ROOK = -9,
    EMPTY = 0,
    WHITE_KING = 1,
    WHITE_QUEEN = 2,
    WHITE_ROOK = 3,
    WHITE_BISHOP = 4,
    WHITE_KNIGHT = 5,
    WHITE_PAWN = 6,
    WHITE_EN_PASSANT_PAWN = 7,
    WHITE_MOVED_KING = 8,
    WHITE_MOVED_ROOK = 9
} Piece;

typedef struct {
    int from_row, from_col;
    int to_row, to_col;
    Piece promoted_to;  // EMPTY se não houver promoção
} move_t;

int possible_moves(board_t b, int i, int j, move_t moves[], int max_moves);
void make_move(board_t b, move_t move);
int is_in_check(board_t b, int is_white);
void init_board(board_t b);

#endif
