#include <iostream>
#include <string>
#include "rules.h"
using namespace std;



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

    static int** tabuleiro = createBoard();

    for (int i = 0;  i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << tabuleiro[i][j] << "\t";
        }
        cout << endl;
    }
    cout << nextMovesOf(4, 5, tabuleiro) << endl;
    int** moves = nextMovesOf(4, 5, tabuleiro);
    for (int i = 0; i < 27; i++) {
        cout << "X: " << moves[i][0] << ", Y: " << moves[i][1] << endl;
    }

    return 0;
}




