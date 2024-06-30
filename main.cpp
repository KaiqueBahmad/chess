#include <iostream>
#include <string>
using namespace std;

enum pieces {
    EMPTY   = 0,
    W_KING,
    W_QUEEN,
    W_ROOK,
    W_KNIGHT,
    W_BISHOP,
    W_PAWN,
    B_KING  = 7,
    B_QUEEN,
    B_ROOK,
    B_KNIGHT,
    B_BISHOP,
    B_PAWN,
};


int **createBoard() {
    int **board = new int*[8];
    for (int i = 0; i < 8; ++i) {
        board[i] = new int[8];
        if (i == 1) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = B_PAWN;
            }
        }
        if (i == 6) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = W_PAWN;
            }
        }
    }
    board[0][0] = B_ROOK;   board[0][1] = B_KNIGHT; board[0][2] = B_BISHOP;
    board[0][3] = B_QUEEN;  board[0][4] = B_KING;   board[0][5] = B_BISHOP;
    board[0][6] = B_KNIGHT; board[0][7] = B_ROOK;
      
    board[7][0] = W_ROOK;   board[7][1] = W_KNIGHT; board[7][2] = W_BISHOP;
    board[7][3] = W_QUEEN;  board[7][4] = W_KING;   board[7][5] = W_BISHOP;
    board[7][6] = W_KNIGHT; board[7][7] = W_ROOK;
    return board;
}


int main() {

    // static int tabuleiro_[][8] = {
    //     {B_ROOK,    B_KNIGHT,   B_BISHOP,   B_QUEEN,    B_KING,     B_BISHOP,   B_KNIGHT,   B_ROOK  },
    //     {B_PAWN,    B_PAWN,     B_PAWN,     B_PAWN,     B_PAWN,     B_PAWN,     B_PAWN,     B_PAWN  },
    //     {EMPTY,     EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY   },
    //     {EMPTY,     EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY   },
    //     {EMPTY,     EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY   },
    //     {EMPTY,     EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY,      EMPTY   },
    //     {W_PAWN,    W_PAWN,     W_PAWN,     W_PAWN,     W_PAWN,     W_PAWN,     W_PAWN,     W_PAWN  },
    //     {W_ROOK,    W_KNIGHT,   W_BISHOP,   W_QUEEN,    W_KING,     W_BISHOP,   W_KNIGHT,   W_ROOK  },
    // };

    int** tabuleiro = createBoard();

    for (int i = 0;  i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << tabuleiro[i][j] << "\t";
        }
        cout << endl;
    }

    return 0;
}




