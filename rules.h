#ifndef RULES_HPP
#define RULES_HPP
#include <algorithm>
#include <iterator>
// Number of moves of moves
// a queen could do if at
// center of the board 
#define MAX_MOVES 27
using namespace std;

enum pieces {
    EMPTY    = 0,
    W_KING   = 1,
    W_QUEEN  = 2,
    W_ROOK   = 3,
    W_KNIGHT = 4,
    W_BISHOP = 5,
    W_PAWN   = 6,
    B_KING   = 7,
    B_QUEEN  = 8,
    B_ROOK   = 10,
    B_KNIGHT = 11,
    B_BISHOP = 12,
    B_PAWN   = 13,
};

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

    board[0][3] = B_ROOK;
    board[5][5] = B_ROOK;
    board[5][4] = W_KING;

    // board[0][0] = B_ROOK;   board[0][1] = B_KNIGHT; board[0][2] = B_BISHOP;
    // board[0][3] = B_QUEEN;  board[0][4] = B_KING;   board[0][5] = B_BISHOP;
    // board[0][6] = B_KNIGHT; board[0][7] = B_ROOK;
      
    // board[7][0] = W_ROOK;   board[7][1] = W_KNIGHT; board[7][2] = W_BISHOP;
    // board[7][3] = W_QUEEN;  board[7][4] = W_KING;   board[7][5] = W_BISHOP;
    // board[7][6] = W_KNIGHT; board[7][7] = W_ROOK;
    return board;
}



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

bool isWhiteKingAtacked(int** board) {
    int x = 0;
    int y = 0;
    // Finding white king
    while (x < 8) {
        while (y < 8) {
            if (board[y][x] == W_KING) {
                break;
            }
            y++;
        }
        if (y < 8 && board[y][x] == W_KING) {
            break;
        }
        y = 0;
        x++;
    }

    // checking for straight line checks from above
    int i = 0;
    bool towerXray = false;
    while (i < y) {
        if (board[i][x] == B_ROOK || board[i][x] == B_QUEEN) {
            towerXray = true;
        } else if (board[i][x] != EMPTY) {
            towerXray = false;
        }
        i++;
    }
    if (towerXray) return towerXray;
    // checking for straight line checks from below
    i = 7;
    towerXray = false;
    while (i > y) {
        if (board[i][x] == B_ROOK || board[i][x] == B_QUEEN) {
            towerXray = true;
        } else if (board[i][x] != EMPTY) {
            towerXray = false;
        }
        i--;
    }
    if (towerXray) return towerXray;
    
    // checking for straight line checks from left
    i = 0;
    towerXray = false;
    while (i < x) {
        if (board[y][i] == B_ROOK || board[y][i] == B_QUEEN) {
            towerXray = true;
        } else if (board[y][i] != EMPTY) {
            towerXray = false;
        }
        i++;
    }
    if (towerXray) return towerXray;
    



    return false;

}

bool isBlackKingAtacked(int** board) {
    return false;
}

bool isEnemyForWhite(int piece) {
    if ((piece >= B_KING && piece <= B_PAWN) || piece == EMPTY) {
        return true;
    }
    return false;
}

bool isEnemyForBlack(int piece) {
    if ((piece >= W_KING && piece <= W_PAWN) || piece == EMPTY) {
        return true;
    }
    return false;
}

int **nextMovesOfWKing(int x, int y, int** board) {
    int** moves = new int*[MAX_MOVES];
    int** tempBoard = copyBoard(board, 8);
    int i = 0;

    // Move up
    if (y-1 >= 0 && isEnemyForWhite(board[y-1][x])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y-1][x];
        tempBoard[y-1][x] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y-1;
            moves[i++][1] = x;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y-1][x] = oldPiece;

    }
    // Move down
    if (y+1 <= 7 && isEnemyForWhite(board[y+1][x])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y+1][x];
        tempBoard[y+1][x] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y+1;
            moves[i++][1] = x;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y+1][x] = oldPiece;
    }

    // Move left
    if (x-1 >= 0 && isEnemyForWhite(board[y][x-1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y][x-1];
        tempBoard[y][x-1] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y;
            moves[i++][1] = x-1;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y][x-1] = oldPiece;
    }

    // Move right
    if (x-1 >= 0 && isEnemyForWhite(board[y][x-1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y][x-1];
        tempBoard[y][x-1] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y;
            moves[i++][1] = x-1;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y][x-1] = oldPiece;
    }

    // Move Up-Left
    if ((y-1 >= 0 && x-1 >= 0) && isEnemyForWhite(board[y-1][x-1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y-1][x-1];
        tempBoard[y-1][x-1] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y-1;
            moves[i++][1] = x-1;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y-1][x-1] = oldPiece;
    }

    // Move  Up-Right
    if ((y-1 >= 0 && x+1 <= 7) && isEnemyForWhite(board[y-1][x+1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y-1][x+1];
        tempBoard[y-1][x+1] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y-1;
            moves[i++][1] = x+1;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y-1][x+1] = oldPiece;
    }

    // Move  Down-Left
    if ((y+1 <= 7 && x-1 >= 0) && isEnemyForWhite(board[y+1][x-1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y+1][x-1];
        tempBoard[y+1][x-1] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y+1;
            moves[i++][1] = x-1;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y+1][x-1] = oldPiece;
    }
    // Move  Down-Right
    if ((y+1 <= 7 && x+1 >= 0) && isEnemyForWhite(board[y+1][x+1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y+1][x+1];
        tempBoard[y+1][x+1] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y+1;
            moves[i++][1] = x+1;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y+1][x+1] = oldPiece;
    }
    deleteBoard(tempBoard, 8);
    while (i < MAX_MOVES) {
        moves[i] = new int[2];
        moves[i][0] = -1;
        moves[i][1] = -1;
        i++;
    }
    return moves;
}

// int **nextMovesOfWQueen(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfWRook(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfWKnight(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfWBishop(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfWPawn(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfBKing(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfBQueen(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfBRook(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfBKnight(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfBBishop(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

// int **nextMovesOfBPawn(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

int **nextMovesOf(int x, int y, int** board) {
    int pieceType = board[y][x];
    cout << board[y][x] << endl;
    switch (pieceType)
    {
    // case B_KING:
    //     return nextMovesOfBKing(x, y, board);
    //     break;
    // case B_QUEEN:
    //     return nextMovesOfBQueen(x, y, board);
    //     break;
    // case B_ROOK:
    //     return nextMovesOfBRook(x, y, board);
    //     break;
    // case B_KNIGHT:
    //     return nextMovesOfBKnight(x, y, board);
    //     break;
    // case B_BISHOP:
    //     return nextMovesOfBBishop(x, y, board);
    //     break;
    // case B_PAWN:
    //     return nextMovesOfBPawn(x, y, board);
    //     break;
    
    case W_KING:
        return nextMovesOfWKing(x, y, board);
        break;
    // case W_QUEEN:
    //     return nextMovesOfWQueen(x, y, board);
    //     break;
    // case W_ROOK:
    //     return nextMovesOfWRook(x, y, board);
    //     break;
    // case W_KNIGHT:
    //     return nextMovesOfWKnight(x, y, board);
    //     break;
    // case W_BISHOP:
    //     return nextMovesOfWBishop(x, y, board);
    //     break;
    // case W_PAWN:
    //     return nextMovesOfWPawn(x, y, board);
    //     break;
    
    default:
        int** moves = new int*[0];
        return moves;
        break;
    }    
}



#endif
