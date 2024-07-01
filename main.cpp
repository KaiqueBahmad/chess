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
    int** moves = nextMovesOf(4, 5, tabuleiro);
    for (int i = 0; i < 27; i++) {
        cout << "X: " << moves[i][0] << ", Y: " << moves[i][1] << endl;
    }

    return 0;
}




