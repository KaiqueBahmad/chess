#ifndef PIECE_HPP
#define PIECE_HPP
#include <vector>

//virtual string getModelo() const = 0;
class Piece {
    int getIdentifier() const = 0;
    std::vector<std::vector<Piece*>> possibleNextStates() const = 0;

    ~Piece() noexcept = default;
};

namespace EPieces {
    enum {
        KING    = 0,
        QUEEN   = 1,
        ROOK    = 2,
        KNIGHT  = 3,
        BISHOOP = 4,
        PAWN    = 5
};
}

#endif