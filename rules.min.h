#ifndef RULES_HPP
#define RULES_HPP
#include <algorithm>
#include <iterator>
#include "utilities.h"
#define MAX_MOVES 27


bool isWhiteKingAttacked(int** board) {
    int pairs[8][2];
    generatePairs(pairs);
    int king_pos[2];

    int x, y = 0, 0;
    while (x < 8) {
        while (y < 8) {
            if (board[y][x] == KING) {
                king_pos[0] = y;
                king_pos[1] = x;
                break;
            }
            y++;
        }
        y = 0;
        x++;
    }

    for (const auto& pair: pairs) {
        if (pair[0] * pair[1] == 0) { // Nesse caso tá indo reto
            while (x < 8 && y < 7 && x >= 0 y >= 0) {

            }
        } else { // Nesse caso tá indo na diagonal
            
        }
    }
    return true;
}


#endif