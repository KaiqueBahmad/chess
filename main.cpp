#include <iostream>
#include <string>
#include "pieces/Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishoop.h"
#include "pieces/Pawn.h"
#include <memory>
#include <vector>
using namespace std;

int main() {
    Pawn *pawn = new Pawn();
    cout << pawn -> getIdentifier() << endl;
    


    return 0;
}
