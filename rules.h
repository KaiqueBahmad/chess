#ifndef RULES_HPP
#define RULES_HPP
#include <algorithm>
#include <iterator>
// Number of moves
// a queen could do if at
// center of the board 
#define MAX_MOVES 27
using namespace std;

enum pieces {
    EMPTY    = 0,
    W_KING,
    W_QUEEN,
    W_ROOK,
    W_KNIGHT,
    W_BISHOP,
    W_EN_PASSANT,
    W_PAWN,
    B_KING,
    B_QUEEN,
    B_ROOK,
    B_KNIGHT,
    B_BISHOP,
    B_EN_PASSANT,
    B_PAWN
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
    board[0][5] = B_ROOK;
    board[5][5] = B_ROOK;
    board[5][4] = B_KING;
    board[6][3] = B_KNIGHT;
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
    int test = 100;
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
    
    // checking for diagonal checks UP-RIGHT
    i = 1;
    while ( i + x < 8 && y-i >= 0  ) {
        if (board[y-i][x+i] != EMPTY) {
            if (board[y-i][x+i] == B_BISHOP || board[y-i][x+i] == B_QUEEN) {
                return true;
            }
            break;
        }
        i++;
    }

    // checking for diagonal checks UP-LEFT
    i = 1;
    while ( x-i >= 0 && y-i >= 0  ) {
        if (board[y-i][x-i] != EMPTY) {
            if (board[y-i][x-i] == B_BISHOP || board[y-i][x-i] == B_QUEEN) {
                return true;
            }
            break;
        }
        i++;
    }

    // checking for diagonal checks DOWN-LEFT
    i = 1;
    while ( x-i >= 0 && y+i < 8  ) {
        if (board[y+i][x-i] != EMPTY) {
            if (board[y+i][x-i] == B_BISHOP || board[y+i][x-i] == B_QUEEN) {
                return true;
            }
            break;
        }
        i++;
    }

    // checking for diagonal checks DOWN-RIGHT
    i = 1;
    while ( x+i < 8 && y+i < 8 ) {
        if (board[y+i][x+i] != EMPTY) {
            if (board[y+i][x+i] == B_BISHOP || board[y+i][x+i] == B_QUEEN) {
                return true;
            }
            break;
        }
        i++;
    }

    // checking for Knight checks
    int knightMoves[8][2] = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2},
        {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}
    };
    i = 0;
    while(i < 8) {
        if (knightMoves[i][0] + x < 0 || knightMoves[i][1] + x > 7 ) {
            i++;
            continue;
        }
        if (knightMoves[i][1] + y < 0 || knightMoves[i][1] + y > 7 ) {
            i++;
            continue;
        }
        // cout << knightMoves[i][0] << " || " << knightMoves[i][1] << endl;
        int cell = board[y+knightMoves[i][1]][x+knightMoves[i][0]];
        if (cell == B_KNIGHT) {
            return true;
        }

        i++;
    }

    return false;

}

bool isBlackKingAtacked(int** board) {
    int x = 0;
    int y = 0;
    // Finding Black king
    while (x < 8) {
        while (y < 8) {
            if (board[y][x] == B_KING) {
                break;
            }
            y++;
        }
        if (y < 8 && board[y][x] == B_KING) {
            break;
        }
        y = 0;
        x++;
    }
    
    // checking for straight line checks from above
    int i = 0;
    bool towerXray = false;
    while (i < y) {
        if (board[i][x] == W_ROOK || board[i][x] == W_QUEEN) {
            towerXray = true;
        } else if (board[i][x] != EMPTY) {
            towerXray = false;
        }
        i++;
    }
    int test = 100;
    if (towerXray) return towerXray;

    // checking for straight line checks from below
    i = 7;
    towerXray = false;
    while (i > y) {
        if (board[i][x] == W_ROOK || board[i][x] == W_QUEEN) {
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
        if (board[y][i] == W_ROOK || board[y][i] == W_QUEEN) {
            towerXray = true;
        } else if (board[y][i] != EMPTY) {
            towerXray = false;
        }
        i++;
    }
    if (towerXray) return towerXray;
    
    // checking for diagonal checks UP-RIGHT
    i = 1;
    while ( i + x < 8 && y-i >= 0  ) {
        if (board[y-i][x+i] != EMPTY) {
            if (board[y-i][x+i] == W_BISHOP || board[y-i][x+i] == W_QUEEN) {
                return true;
            }
            break;
        }
        i++;
    }

    // checking for diagonal checks UP-LEFT
    i = 1;
    while ( x-i >= 0 && y-i >= 0  ) {
        if (board[y-i][x-i] != EMPTY) {
            if (board[y-i][x-i] == W_BISHOP || board[y-i][x-i] == W_QUEEN) {
                return true;
            }
            break;
        }
        i++;
    }

    // checking for diagonal checks DOWN-LEFT
    i = 1;
    while ( x-i >= 0 && y+i < 8  ) {
        if (board[y+i][x-i] != EMPTY) {
            if (board[y+i][x-i] == W_BISHOP || board[y+i][x-i] == W_QUEEN) {
                return true;
            }
            break;
        }
        i++;
    }

    // checking for diagonal checks DOWN-RIGHT
    i = 1;
    while ( x+i < 8 && y+i < 8 ) {
        if (board[y+i][x+i] != EMPTY) {
            if (board[y+i][x+i] == W_BISHOP || board[y+i][x+i] == W_QUEEN) {
                return true;
            }
            break;
        }
        i++;
    }

    // checking for Knight checks
    int knightMoves[8][2] = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2},
        {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}
    };
    i = 0;
    while(i < 8) {
        if (knightMoves[i][0] + x < 0 || knightMoves[i][1] + x > 7 ) {
            i++;
            continue;
        }
        if (knightMoves[i][1] + y < 0 || knightMoves[i][1] + y > 7 ) {
            i++;
            continue;
        }
        // cout << knightMoves[i][0] << " || " << knightMoves[i][1] << endl;
        int cell = board[y+knightMoves[i][1]][x+knightMoves[i][0]];
        if (cell == W_KNIGHT) {
            return true;
        }

        i++;
    }

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
            moves[i][0] = x;
            moves[i++][1] = y-1;
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
            moves[i][0] = x;
            moves[i++][1] = y+1;
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
            moves[i][0] = x-1;
            moves[i++][1] = y;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y][x-1] = oldPiece;
    }

    // Move right
    if (x+1 < 8 && isEnemyForWhite(board[y][x+1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y][x+1];
        tempBoard[y][x+1] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x+1;
            moves[i++][1] = y;
        }
        tempBoard[y][x] = W_KING;
        tempBoard[y][x+1] = oldPiece;
    }

    // Move Up-Left
    if ((y-1 >= 0 && x-1 >= 0) && isEnemyForWhite(board[y-1][x-1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y-1][x-1];
        tempBoard[y-1][x-1] = W_KING;
        if (!isWhiteKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x-1;
            moves[i++][1] = y-1;
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
            moves[i][0] = x+1;
            moves[i++][1] = y-1;
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
            moves[i][0] = x-1;
            moves[i++][1] = y+1;
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

int **nextMovesOfBKing(int x, int y, int** board) {
    int** moves = new int*[MAX_MOVES];
    int** tempBoard = copyBoard(board, 8);
    int i = 0;

    // Move up
    if (y-1 >= 0 && isEnemyForBlack(board[y-1][x])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y-1][x];
        tempBoard[y-1][x] = B_KING;
        if (!isBlackKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x;
            moves[i++][1] = y-1;
        }
        tempBoard[y][x] = B_KING;
        tempBoard[y-1][x] = oldPiece;

    }
    // Move down
    if (y+1 <= 7 && isEnemyForBlack(board[y+1][x])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y+1][x];
        tempBoard[y+1][x] = B_KING;
        if (!isBlackKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x;
            moves[i++][1] = y+1;
        }
        tempBoard[y][x] = B_KING;
        tempBoard[y+1][x] = oldPiece;
    }

    // Move left
    if (x-1 >= 0 && isEnemyForBlack(board[y][x-1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y][x-1];
        tempBoard[y][x-1] = B_KING;
        if (!isBlackKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x-1;
            moves[i++][1] = y;
        }
        tempBoard[y][x] = B_KING;
        tempBoard[y][x-1] = oldPiece;
    }

    // Move right
    if (x+1 < 8 && isEnemyForBlack(board[y][x+1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y][x+1];
        tempBoard[y][x+1] = B_KING;
        if (!isBlackKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x+1;
            moves[i++][1] = y;
        }
        tempBoard[y][x] = B_KING;
        tempBoard[y][x+1] = oldPiece;
    }

    // Move Up-Left
    if ((y-1 >= 0 && x-1 >= 0) && isEnemyForBlack(board[y-1][x-1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y-1][x-1];
        tempBoard[y-1][x-1] = B_KING;
        if (!isBlackKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x-1;
            moves[i++][1] = y-1;
        }
        tempBoard[y][x] = B_KING;
        tempBoard[y-1][x-1] = oldPiece;
    }

    // Move  Up-Right
    if ((y-1 >= 0 && x+1 <= 7) && isEnemyForBlack(board[y-1][x+1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y-1][x+1];
        tempBoard[y-1][x+1] = B_KING;
        if (!isBlackKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x+1;
            moves[i++][1] = y-1;
        }
        tempBoard[y][x] = B_KING;
        tempBoard[y-1][x+1] = oldPiece;
    }

    // Move  Down-Left
    if ((y+1 <= 7 && x-1 >= 0) && isEnemyForBlack(board[y+1][x-1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y+1][x-1];
        tempBoard[y+1][x-1] = B_KING;
        if (!isBlackKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = x-1;
            moves[i++][1] = y+1;
        }
        tempBoard[y][x] = B_KING;
        tempBoard[y+1][x-1] = oldPiece;
    }
    // Move  Down-Right
    if ((y+1 <= 7 && x+1 >= 0) && isEnemyForBlack(board[y+1][x+1])) {
        tempBoard[y][x] = EMPTY;
        int oldPiece = tempBoard[y+1][x+1];
        tempBoard[y+1][x+1] = B_KING;
        if (!isBlackKingAtacked(tempBoard)) {
            moves[i] = new int[2];
            moves[i][0] = y+1;
            moves[i++][1] = x+1;
        }
        tempBoard[y][x] = B_KING;
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

// int **nextMovesOfBQueen(int x, int y, int** board) {
//     int** moves = new int*[MAX_MOVES];

// }

int **nextMovesOfBRook(int x, int y, int** board) {
    int** moves = new int*[MAX_MOVES];
    int n = 0;
    int** tempBoard = copyBoard(board, 8);
    int i = 1;
    // Straight down moves
    while (i + y < 8) {
        if (board[y+i][x] == EMPTY) {
            tempBoard[y][x] = EMPTY;
            int oldPiece = tempBoard[y+i][x];
            tempBoard[y+i][x] = B_ROOK;
            if (!isBlackKingAtacked(tempBoard)) {
                moves[n] = new int[2];
                moves[n][0] = x;
                moves[n++][1] = y+i;
            }
            tempBoard[y][x] = B_ROOK;
            tempBoard[y+i][x] = oldPiece;
            i++;
            continue;
        }
        cout << x << "  " << y+i << endl;
        cout << (board[y+i][x] == EMPTY) << endl;
        if (isEnemyForBlack(board[y+i][x])) {
            tempBoard[y][x] = EMPTY;
            int oldPiece = tempBoard[y+i][x];
            tempBoard[y+i][x] = B_ROOK;
            if (!isBlackKingAtacked(tempBoard)) {
                moves[n] = new int[2];
                moves[n][0] = x;
                moves[n++][1] = y+i;
            }
            tempBoard[y][x] = B_ROOK;
            tempBoard[y+i][x] = oldPiece;
        }
        break;
    }
    
    // Straight up moves
    i = 1;
    while(y - i >= 0) {
        if (board[y-i][x] == EMPTY) {
            tempBoard[y][x] = EMPTY;
            int oldPiece = tempBoard[y-i][x];
            tempBoard[y-i][x] = B_ROOK;
            if (!isBlackKingAtacked(tempBoard)) {
                moves[n] = new int[2];
                moves[n][0] = x;
                moves[n++][1] = y-i;
            }
            tempBoard[y][x] = B_ROOK;
            tempBoard[y-i][x] = oldPiece;
            i++;
            continue;
        }
        if (isEnemyForBlack(board[y-i][x])) {
            tempBoard[y][x] = EMPTY;
            int oldPiece = tempBoard[y-i][x];
            tempBoard[y-i][x] = B_ROOK;
            if (!isBlackKingAtacked(tempBoard)) {
                moves[n] = new int[2];
                moves[n][0] = x;
                moves[n++][1] = y-i;
            }
            tempBoard[y][x] = B_ROOK;
            tempBoard[y-i][x] = oldPiece;
        }
        break;
    }

    // Straight right moves
    i = 1;
    while(x + i < 8) {
        if (board[y][x+i] == EMPTY) {
            tempBoard[y][x] = EMPTY;
            int oldPiece = tempBoard[y][x+i];
            tempBoard[y][x+i] = B_ROOK;
            if (!isBlackKingAtacked(tempBoard)) {
                moves[n] = new int[2];
                moves[n][0] = x+i;
                moves[n++][1] = y;
            }
            tempBoard[y][x] = B_ROOK;
            tempBoard[y][x+i] = oldPiece;
            i++;
            continue;
        }
        if (isEnemyForBlack(board[y][x+i])) {
            tempBoard[y][x] = EMPTY;
            int oldPiece = tempBoard[y][x+i];
            tempBoard[y][x+i] = B_ROOK;
            if (!isBlackKingAtacked(tempBoard)) {
                moves[n] = new int[2];
                moves[n][0] = x+i;
                moves[n++][1] = y;
            }
            tempBoard[y][x] = B_ROOK;
            tempBoard[y][x+i] = oldPiece;
        }
        break;
    }

    
   // Straight left moves
    i = 1;
    while(x - i >= 0) {
        if (board[y][x-i] == EMPTY) {
            tempBoard[y][x] = EMPTY;
            int oldPiece = tempBoard[y][x-i];
            tempBoard[y][x-i] = B_ROOK;
            if (!isBlackKingAtacked(tempBoard)) {
                moves[n] = new int[2];
                moves[n][0] = x-i;
                moves[n++][1] = y;
            }
            tempBoard[y][x] = B_ROOK;
            tempBoard[y][x-i] = oldPiece;
            i++;
            continue;
        }
        if (isEnemyForBlack(board[y][x-i])) {
            tempBoard[y][x] = EMPTY;
            int oldPiece = tempBoard[y][x-i];
            tempBoard[y][x-i] = B_ROOK;
            if (!isBlackKingAtacked(tempBoard)) {
                moves[n] = new int[2];
                moves[n][0] = x-i;
                moves[n++][1] = y;
            }
            tempBoard[y][x] = B_ROOK;
            tempBoard[y][x-i] = oldPiece;
        }
        break;
        }

    deleteBoard(tempBoard, 8);
    while (n < MAX_MOVES) {
        moves[n] = new int[2];
        moves[n][0] = -1;
        moves[n][1] = -1;
        n++;
    }
    return moves;

}

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
    switch (pieceType)
    {
    case B_KING:
        return nextMovesOfBKing(x, y, board);
        break;
    // case B_QUEEN:
    //     return nextMovesOfBQueen(x, y, board);
    //     break;
    case B_ROOK:
        return nextMovesOfBRook(x, y, board);
        break;
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
