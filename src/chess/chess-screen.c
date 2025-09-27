#include "chess-screen.h"
#include <stdlib.h>

static const PixelArt king_black = {
    {TRANSPARENT, TRANSPARENT, WHITE, BLACK, BLACK, WHITE, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, WHITE, BLACK, WHITE, WHITE, BLACK, WHITE, TRANSPARENT},
    {WHITE, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, WHITE},
    {WHITE, BLACK, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE},
    {BLACK, WHITE, BLACK, WHITE, WHITE, BLACK, WHITE, BLACK},
    {BLACK, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}
};

static const PixelArt queen_black = {
    {TRANSPARENT, BLACK, TRANSPARENT, BLACK, BLACK, TRANSPARENT, BLACK, TRANSPARENT},
    {BLACK, WHITE, BLACK, WHITE, WHITE, BLACK, WHITE, BLACK},
    {BLACK, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, BLACK},
    {WHITE, BLACK, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE},
    {BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE, BLACK},
    {BLACK, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}
};

static const PixelArt rook_black = {
    {BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
    {WHITE, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, WHITE},
    {BLACK, WHITE, BLACK, WHITE, WHITE, BLACK, WHITE, BLACK},
    {BLACK, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, BLACK},
    {BLACK, BLACK, BLACK, WHITE, WHITE, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}
};

static const PixelArt bishop_black = {
    {TRANSPARENT, TRANSPARENT, TRANSPARENT, BLACK, BLACK, TRANSPARENT, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, TRANSPARENT, BLACK, WHITE, WHITE, BLACK, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, TRANSPARENT},
    {BLACK, WHITE, BLACK, WHITE, WHITE, BLACK, WHITE, BLACK},
    {BLACK, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, BLACK},
    {BLACK, BLACK, BLACK, WHITE, WHITE, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}
};

static const PixelArt knight_black = {
    {TRANSPARENT, TRANSPARENT, BLACK, BLACK, WHITE, TRANSPARENT, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, BLACK, WHITE, BLACK, BLACK, WHITE, TRANSPARENT, TRANSPARENT},
    {BLACK, WHITE, BLACK, WHITE, WHITE, BLACK, BLACK, TRANSPARENT},
    {BLACK, BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, BLACK},
    {BLACK, BLACK, BLACK, WHITE, WHITE, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}
};

static const PixelArt pawn_black = {
    {TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, TRANSPARENT, TRANSPARENT, BLACK, BLACK, TRANSPARENT, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, TRANSPARENT, BLACK, WHITE, WHITE, BLACK, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, TRANSPARENT, BLACK, BLACK, BLACK, BLACK, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, TRANSPARENT, TRANSPARENT, BLACK, BLACK, TRANSPARENT, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, TRANSPARENT, BLACK, BLACK, BLACK, BLACK, TRANSPARENT, TRANSPARENT},
    {TRANSPARENT, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, TRANSPARENT},
    {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}
};

// espaços reservados para peças brancas
static PixelArt king_white = {};
static PixelArt queen_white = {};
static PixelArt rook_white = {};
static PixelArt bishop_white = {};
static PixelArt knight_white = {};
static PixelArt pawn_white = {};

Color invertColor(Color c) {
    switch (c) {
        case BLACK: return WHITE;
        case WHITE: return BLACK;
        case TRANSPARENT: return TRANSPARENT;  
        case GREEN: return GREEN;              
        case ORANGE: return ORANGE;            
        default: return c;
    }
}

void generateInvertedPixelArt(const PixelArt* source, PixelArt* dest) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            (*dest)[i][j] = invertColor((*source)[i][j]);
        }
    }
}

int isPixelArtEmpty(const PixelArt* art) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((*art)[i][j] != 0) return 0;  
        }
    }
    return 1;  
}

const PixelArt* pieceToPixelArt(Piece p) {
    if (p > 0) {
        PixelArt* white_art = NULL;
        const PixelArt* black_art = NULL;

        switch (p) {
            case WHITE_KING:   white_art = &king_white;   black_art = &king_black;   break;
            case WHITE_QUEEN:  white_art = &queen_white;  black_art = &queen_black;  break;
            case WHITE_ROOK:   white_art = &rook_white;   black_art = &rook_black;   break;
            case WHITE_BISHOP: white_art = &bishop_white; black_art = &bishop_black; break;
            case WHITE_KNIGHT: white_art = &knight_white; black_art = &knight_black; break;
            case WHITE_PAWN:   white_art = &pawn_white;   black_art = &pawn_black;   break;
            default: return &pawn_black;  
        }

        if (isPixelArtEmpty(white_art)) {
            generateInvertedPixelArt(black_art, white_art);
        }
        return white_art;
    }

    switch (p) {
        case BLACK_KING:   return &king_black;
        case BLACK_QUEEN:  return &queen_black;
        case BLACK_ROOK:   return &rook_black;
        case BLACK_BISHOP: return &bishop_black;
        case BLACK_KNIGHT: return &knight_black;
        case BLACK_PAWN:   return &pawn_black;
        default:           return &pawn_black;
    }
}
