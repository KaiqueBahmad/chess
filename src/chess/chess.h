#ifndef CHESS_H
#define CHESS_H


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
} chess_Piece;

typedef chess_Piece chess_board_t[8][8];

typedef struct {
    int from_row, from_col;
    int to_row, to_col;
    chess_Piece promoted_to;  // EMPTY se não houver promoção
} chess_move_t;

int chess_possible_moves(chess_board_t b, int i, int j, chess_move_t moves[], int max_moves);
void chess_make_move(chess_board_t b, chess_move_t move);
int chess_is_in_check(chess_board_t b, int is_white);
void chess_init_board(chess_board_t b);
#endif
