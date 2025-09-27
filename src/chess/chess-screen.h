#ifndef CHESS_SCREEN_H
#define CHESS_SCREEN_H

#include "chess.h"

typedef enum {
  GREEN = -3,
  ORANGE = -2,
  TRANSPARENT = -1,
  BLACK = 0,
  WHITE = 1
} Color;

typedef Color PixelArt[8][8];
typedef Color Screen[64][64];

Color invertColor(Color c);
void generateInvertedPixelArt(const PixelArt* source, PixelArt* dest);
int isPixelArtEmpty(const PixelArt* art);
const PixelArt* pieceToPixelArt(Piece p);

#endif
