#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include "pieces.h"


/* Utilities */
void generatePairs(int pairs[9][2]) {
    int values[] = {-1, 0, 1};
    int index = 0;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            pairs[index][0] = values[i];
            pairs[index][1] = values[j];
            ++index;
        }
    }
}
// Remove this later, just example of use
// void printPairs(const int pairs[SIZE][2]) {
//     for (int i = 0; i < SIZE; ++i) {
//         std::cout << '[' << pairs[i][0] << ", " << pairs[i][1] << "]\n";
//     }
// }

int** copyBoard(int** board, int size) {
    int** tempBoard = new int*[size];
    for (int i = 0; i < size; ++i) {
        tempBoard[i] = new int[size];
        std::copy(board[i], board[i] + size, tempBoard[i]);
    }
    return tempBoard;
}

void deleteBoard(int** board, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] board[i];
    }
    delete[] board;
}

void revertMatrix(int matrix[8][8]) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            matrix[i][j] = -matrix[i][j];
        }
    }
}

// This can may be also be deleted later when deployed to ardunio
int **createBoard() {
    int **board = new int*[8];
    for (int i = 0; i < 8; ++i) {
        board[i] = new int[8];
        for (int j = 0; j < 8; j++) {
            board[i][j] = 0;
        }
        // if (i == 1) {
        //     for (int j = 0; j < 8; j++) {
        //         board[i][j] = B_PAWN;
        //     }
        // }
        // if (i == 6) {
        //     for (int j = 0; j < 8; j++) {
        //         board[i][j] = W_PAWN;
        //     }
        // }
    }

    board[0][3] = -ROOK;
    board[0][5] = -ROOK;
    board[5][5] = -ROOK;
    board[5][4] = KING;
    board[6][3] = -KNIGHT;
    // board[0][0] = B_ROOK;   board[0][1] = B_KNIGHT; board[0][2] = B_BISHOP;
    // board[0][3] = B_QUEEN;  board[0][4] = B_KING;   board[0][5] = B_BISHOP;
    // board[0][6] = B_KNIGHT; board[0][7] = B_ROOK;
      
    // board[7][0] = W_ROOK;   board[7][1] = W_KNIGHT; board[7][2] = W_BISHOP;
    // board[7][3] = W_QUEEN;  board[7][4] = W_KING;   board[7][5] = W_BISHOP;
    // board[7][6] = W_KNIGHT; board[7][7] = W_ROOK;
    return board;
}
#endif