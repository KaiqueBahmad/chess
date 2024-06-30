#ifndef PIECE_HPP
#define PIECE_HPP

//virtual string getModelo() const = 0;
class Piece {
public:
    vector<std::vector<Piece*>>& board;
    Piece (int type, vector<std::vector<Piece*>>& board_) {
        identifier = Piece_code(type);
        if (type > 5 || type < 0) {
            identifier = EMPTY;
        }
        board = board_;
    }
    int getIdentifier() {
        return this -> identifier;
    };
    std::vector<std::vector<Piece*>> getPossibleMoves();
private:
    Piece_code identifier = Piece_code(6);
};


enum Piece_code{
    KING    = 0,
    QUEEN   = 1,
    ROOK    = 2,
    KNIGHT  = 3,
    BISHOOP = 4,
    PAWN    = 5,
    EMPTY   = 6
};


#endif