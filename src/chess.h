#ifndef CHESS_H
#define CHESS_H

// Type definitions
typedef signed char board_t[8][8];

typedef struct {
    int from_row, from_col;
    int to_row, to_col;
    signed char piece;
    signed char captured; // 0 if no capture, otherwise the captured piece
} move_t;

// Function declarations
static inline int valid(int r, int c) { 
    return (unsigned)r < 8 && (unsigned)c < 8; 
}

int possible_moves(board_t b, int i, int j, move_t moves[], int max_moves);
int is_at_check(board_t b);
// void generate_random_position(board_t board);
// void simple_benchmark(int num_positions);

#endif // CHESS_H