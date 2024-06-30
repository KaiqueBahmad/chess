#ifndef PAWN_HPP
#define PAWN_HPP
#include "Piece.h"
#include <vector>

class Pawn: public Piece {
    public:


        int getIdentifier() const override {
            return EPieces::PAWN;
        }

        std::vector<std::vector<Piece*>> possibleNextStates() const override {
            std::vector<std::vector<Piece*>> nextState(8, std::vector<Piece*>(8, nullptr));
            int i = 0;
            while (i <8) {
                nextState[1][i] = Pawn();
            }

            return nextState;
        }
        ~Pawn() noexcept = default;

};

#endif