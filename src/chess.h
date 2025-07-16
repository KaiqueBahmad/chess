#ifndef CHESS_H
#define CHESS_H

// Type definitions
typedef signed char board_t[8][8];

// Function declarations
static inline int valid(int r, int c) { 
    return (unsigned)r < 8 && (unsigned)c < 8; 
}

int possible_moves(board_t b, int i, int j);
int is_at_check(board_t b);
// void generate_random_position(board_t board);
// void simple_benchmark(int num_positions);

#endif // CHESS_H