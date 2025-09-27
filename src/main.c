#include "chess.h"
#include "chess-screen.h"
#include <stdio.h>

int main() {
  Piece chessboard[8][8] = {
    {BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK},
    {BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN},
    {WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}
  };

  for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
          printf("%d ", chessboard[i][j]);
      }
      printf("\n");
  }

  printf("-----------\n");

  const PixelArt* art = pieceToPixelArt(BLACK_KING);
  for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
          printf("%d\t", (*art)[i][j]);
      }
      printf("\n");
  }

  return 0;
}
