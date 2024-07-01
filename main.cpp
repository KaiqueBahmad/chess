#include <iostream>
#include <string>
#include "rules.h"
using namespace std;



int main() {
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
        cout << "X: " << moves[i][1] << ", Y: " << moves[i][0] << endl;
    }

    return 0;
}




