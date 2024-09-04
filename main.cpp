#include <iostream>
#include <string>
#include "rules.min.h"
using namespace std;



int main() {
    static int** tabuleiro = createBoard();
    for (int i = 0;  i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << tabuleiro[i][j] << "\t";
        }
        cout << endl << endl;
    }
    // int** moves = nextMovesOf(5, 0, tabuleiro);
    // for (int i = 0; i < 27; i++) {
    //     cout << "X: " << moves[i][0] << ", Y: " << moves[i][1] << endl;
    // }
    isWhiteKingAttacked(tabuleiro);
    return 0;
}




