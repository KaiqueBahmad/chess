#include "chess.h"
#include <stdlib.h>

// Funções auxiliares
static int is_white_piece(chess_Piece p) {
    return p > 0;
}

static int is_black_piece(chess_Piece p) {
    return p < 0;
}

static int is_valid_pos(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

static int is_enemy(chess_Piece p1, chess_Piece p2) {
    return (is_white_piece(p1) && is_black_piece(p2)) ||
           (is_black_piece(p1) && is_white_piece(p2));
}

static int is_ally(chess_Piece p1, chess_Piece p2) {
    if (p1 == EMPTY || p2 == EMPTY) return 0;
    return (is_white_piece(p1) && is_white_piece(p2)) ||
           (is_black_piece(p1) && is_black_piece(p2));
}

// Normaliza peça (remove flags de movimento)
static chess_Piece normalize_piece(chess_Piece p) {
    if (p == BLACK_MOVED_KING) return BLACK_KING;
    if (p == BLACK_MOVED_ROOK) return BLACK_ROOK;
    if (p == WHITE_MOVED_KING) return WHITE_KING;
    if (p == WHITE_MOVED_ROOK) return WHITE_ROOK;
    if (p == BLACK_EN_PASSANT_PAWN) return BLACK_PAWN;
    if (p == WHITE_EN_PASSANT_PAWN) return WHITE_PAWN;
    return p;
}

// Adiciona movimento à lista
static int add_move(chess_move_t moves[], int count, int max, int fr, int fc, int tr, int tc, chess_Piece promo) {
    if (count >= max) return count;
    moves[count].from_row = fr;
    moves[count].from_col = fc;
    moves[count].to_row = tr;
    moves[count].to_col = tc;
    moves[count].promoted_to = promo;
    return count + 1;
}

// Movimentos do peão
static int pawn_moves(chess_board_t b, int row, int col, chess_move_t moves[], int count, int max) {
    chess_Piece piece = b[row][col];
    int is_white = is_white_piece(piece);
    int dir = is_white ? -1 : 1;
    int start_row = is_white ? 6 : 1;
    int promo_row = is_white ? 0 : 7;

    // Movimento para frente
    if (is_valid_pos(row + dir, col) && b[row + dir][col] == EMPTY) {
        if (row + dir == promo_row) {
            // Promoção
            chess_Piece queen = is_white ? WHITE_QUEEN : BLACK_QUEEN;
            chess_Piece rook = is_white ? WHITE_ROOK : BLACK_ROOK;
            chess_Piece bishop = is_white ? WHITE_BISHOP : BLACK_BISHOP;
            chess_Piece knight = is_white ? WHITE_KNIGHT : BLACK_KNIGHT;
            count = add_move(moves, count, max, row, col, row + dir, col, queen);
            count = add_move(moves, count, max, row, col, row + dir, col, rook);
            count = add_move(moves, count, max, row, col, row + dir, col, bishop);
            count = add_move(moves, count, max, row, col, row + dir, col, knight);
        } else {
            count = add_move(moves, count, max, row, col, row + dir, col, EMPTY);
        }

        // Movimento duplo inicial
        if (row == start_row && b[row + 2*dir][col] == EMPTY) {
            count = add_move(moves, count, max, row, col, row + 2*dir, col, EMPTY);
        }
    }

    // Capturas diagonais
    for (int dc = -1; dc <= 1; dc += 2) {
        if (is_valid_pos(row + dir, col + dc)) {
            chess_Piece target = b[row + dir][col + dc];
            if (is_enemy(piece, target)) {
                if (row + dir == promo_row) {
                    chess_Piece queen = is_white ? WHITE_QUEEN : BLACK_QUEEN;
                    chess_Piece rook = is_white ? WHITE_ROOK : BLACK_ROOK;
                    chess_Piece bishop = is_white ? WHITE_BISHOP : BLACK_BISHOP;
                    chess_Piece knight = is_white ? WHITE_KNIGHT : BLACK_KNIGHT;
                    count = add_move(moves, count, max, row, col, row + dir, col + dc, queen);
                    count = add_move(moves, count, max, row, col, row + dir, col + dc, rook);
                    count = add_move(moves, count, max, row, col, row + dir, col + dc, bishop);
                    count = add_move(moves, count, max, row, col, row + dir, col + dc, knight);
                } else {
                    count = add_move(moves, count, max, row, col, row + dir, col + dc, EMPTY);
                }
            }
        }
    }

    // En passant
    for (int dc = -1; dc <= 1; dc += 2) {
        if (is_valid_pos(row, col + dc)) {
            chess_Piece adj = b[row][col + dc];
            if ((is_white && adj == BLACK_EN_PASSANT_PAWN) ||
                (!is_white && adj == WHITE_EN_PASSANT_PAWN)) {
                count = add_move(moves, count, max, row, col, row + dir, col + dc, EMPTY);
            }
        }
    }

    return count;
}

// Movimentos em linha reta (torre e rainha)
static int straight_moves(chess_board_t b, int row, int col, chess_move_t moves[], int count, int max) {
    chess_Piece piece = b[row][col];
    int dirs[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

    for (int d = 0; d < 4; d++) {
        for (int dist = 1; dist < 8; dist++) {
            int nr = row + dirs[d][0] * dist;
            int nc = col + dirs[d][1] * dist;

            if (!is_valid_pos(nr, nc)) break;

            chess_Piece target = b[nr][nc];
            if (target == EMPTY) {
                count = add_move(moves, count, max, row, col, nr, nc, EMPTY);
            } else if (is_enemy(piece, target)) {
                count = add_move(moves, count, max, row, col, nr, nc, EMPTY);
                break;
            } else {
                break;
            }
        }
    }
    return count;
}

// Movimentos diagonais (bispo e rainha)
static int diagonal_moves(chess_board_t b, int row, int col, chess_move_t moves[], int count, int max) {
    chess_Piece piece = b[row][col];
    int dirs[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};

    for (int d = 0; d < 4; d++) {
        for (int dist = 1; dist < 8; dist++) {
            int nr = row + dirs[d][0] * dist;
            int nc = col + dirs[d][1] * dist;

            if (!is_valid_pos(nr, nc)) break;

            chess_Piece target = b[nr][nc];
            if (target == EMPTY) {
                count = add_move(moves, count, max, row, col, nr, nc, EMPTY);
            } else if (is_enemy(piece, target)) {
                count = add_move(moves, count, max, row, col, nr, nc, EMPTY);
                break;
            } else {
                break;
            }
        }
    }
    return count;
}

// Movimentos do cavalo
static int knight_moves(chess_board_t b, int row, int col, chess_move_t moves[], int count, int max) {
    chess_Piece piece = b[row][col];
    int offsets[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};

    for (int i = 0; i < 8; i++) {
        int nr = row + offsets[i][0];
        int nc = col + offsets[i][1];

        if (is_valid_pos(nr, nc)) {
            chess_Piece target = b[nr][nc];
            if (target == EMPTY || is_enemy(piece, target)) {
                count = add_move(moves, count, max, row, col, nr, nc, EMPTY);
            }
        }
    }
    return count;
}

// Movimentos do rei
static int king_moves(chess_board_t b, int row, int col, chess_move_t moves[], int count, int max) {
    chess_Piece piece = b[row][col];
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

    for (int i = 0; i < 8; i++) {
        int nr = row + dirs[i][0];
        int nc = col + dirs[i][1];

        if (is_valid_pos(nr, nc)) {
            chess_Piece target = b[nr][nc];
            if (target == EMPTY || is_enemy(piece, target)) {
                count = add_move(moves, count, max, row, col, nr, nc, EMPTY);
            }
        }
    }

    // Roque (será validado posteriormente em is_legal_move)
    int is_white = is_white_piece(piece);
    if (piece == WHITE_KING || piece == BLACK_KING) {
        // Roque curto (lado do rei)
        if (b[row][col+1] == EMPTY && b[row][col+2] == EMPTY) {
            chess_Piece rook = is_white ? WHITE_ROOK : BLACK_ROOK;
            if (b[row][7] == rook) {
                count = add_move(moves, count, max, row, col, row, col+2, EMPTY);
            }
        }
        // Roque longo (lado da rainha)
        if (b[row][col-1] == EMPTY && b[row][col-2] == EMPTY && b[row][col-3] == EMPTY) {
            chess_Piece rook = is_white ? WHITE_ROOK : BLACK_ROOK;
            if (b[row][0] == rook) {
                count = add_move(moves, count, max, row, col, row, col-2, EMPTY);
            }
        }
    }

    return count;
}

// Verifica se um movimento é legal (não deixa o rei em xeque)
static int is_legal_move(chess_board_t b, chess_move_t move) {
    chess_board_t temp;

    // Copia o tabuleiro
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            temp[i][j] = b[i][j];
        }
    }

    // Faz o movimento temporariamente
    chess_Piece piece = temp[move.from_row][move.from_col];
    int is_white = is_white_piece(piece);

    temp[move.to_row][move.to_col] = piece;
    temp[move.from_row][move.from_col] = EMPTY;

    // Captura en passant
    chess_Piece norm = normalize_piece(piece);
    if ((norm == WHITE_PAWN || norm == BLACK_PAWN) &&
        move.from_col != move.to_col &&
        b[move.to_row][move.to_col] == EMPTY) {
        temp[move.from_row][move.to_col] = EMPTY;
    }

    // Verifica se ficou em xeque
    return !chess_is_in_check(temp, is_white);
}

int chess_possible_moves(chess_board_t b, int i, int j, chess_move_t moves[], int max_moves) {
    if (!is_valid_pos(i, j)) return 0;

    chess_Piece piece = normalize_piece(b[i][j]);
    if (piece == EMPTY) return 0;

    chess_move_t all_moves[100];
    int count = 0;

    switch (piece) {
        case WHITE_PAWN:
        case BLACK_PAWN:
            count = pawn_moves(b, i, j, all_moves, count, 100);
            break;
        case WHITE_ROOK:
        case BLACK_ROOK:
            count = straight_moves(b, i, j, all_moves, count, 100);
            break;
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            count = diagonal_moves(b, i, j, all_moves, count, 100);
            break;
        case WHITE_QUEEN:
        case BLACK_QUEEN:
            count = straight_moves(b, i, j, all_moves, count, 100);
            count = diagonal_moves(b, i, j, all_moves, count, 100);
            break;
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            count = knight_moves(b, i, j, all_moves, count, 100);
            break;
        case WHITE_KING:
        case BLACK_KING:
            count = king_moves(b, i, j, all_moves, count, 100);
            break;
        default:
            break;
    }

    // Filtra apenas movimentos legais (que não deixam o rei em xeque)
    int legal_count = 0;
    for (int k = 0; k < count && legal_count < max_moves; k++) {
        if (is_legal_move(b, all_moves[k])) {
            moves[legal_count++] = all_moves[k];
        }
    }

    return legal_count;
}

void chess_make_move(chess_board_t b, chess_move_t move) {
    chess_Piece piece = b[move.from_row][move.from_col];
    chess_Piece norm_piece = normalize_piece(piece);

    // Remove flags de en passant anteriores
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (b[i][j] == WHITE_EN_PASSANT_PAWN) b[i][j] = WHITE_PAWN;
            if (b[i][j] == BLACK_EN_PASSANT_PAWN) b[i][j] = BLACK_PAWN;
        }
    }

    // Move a peça
    b[move.to_row][move.to_col] = piece;
    b[move.from_row][move.from_col] = EMPTY;

    // Promoção
    if (move.promoted_to != EMPTY) {
        b[move.to_row][move.to_col] = move.promoted_to;
    }

    // Marca rei/torre como movida
    if (norm_piece == WHITE_KING) b[move.to_row][move.to_col] = WHITE_MOVED_KING;
    if (norm_piece == BLACK_KING) b[move.to_row][move.to_col] = BLACK_MOVED_KING;
    if (norm_piece == WHITE_ROOK) b[move.to_row][move.to_col] = WHITE_MOVED_ROOK;
    if (norm_piece == BLACK_ROOK) b[move.to_row][move.to_col] = BLACK_MOVED_ROOK;

    // En passant: movimento duplo de peão
    if ((norm_piece == WHITE_PAWN || norm_piece == BLACK_PAWN) &&
        abs(move.to_row - move.from_row) == 2) {
        if (norm_piece == WHITE_PAWN) {
            b[move.to_row][move.to_col] = WHITE_EN_PASSANT_PAWN;
        } else {
            b[move.to_row][move.to_col] = BLACK_EN_PASSANT_PAWN;
        }
    }

    // Captura en passant
    if ((norm_piece == WHITE_PAWN || norm_piece == BLACK_PAWN) &&
        move.from_col != move.to_col &&
        b[move.to_row][move.to_col] == (norm_piece == WHITE_PAWN ? WHITE_EN_PASSANT_PAWN : BLACK_EN_PASSANT_PAWN)) {
        b[move.from_row][move.to_col] = EMPTY;
    }

    // Roque
    if ((norm_piece == WHITE_KING || norm_piece == BLACK_KING) &&
        abs(move.to_col - move.from_col) == 2) {
        if (move.to_col > move.from_col) {
            // Roque curto
            chess_Piece rook = b[move.to_row][7];
            b[move.to_row][5] = (norm_piece == WHITE_KING) ? WHITE_MOVED_ROOK : BLACK_MOVED_ROOK;
            b[move.to_row][7] = EMPTY;
        } else {
            // Roque longo
            chess_Piece rook = b[move.to_row][0];
            b[move.to_row][3] = (norm_piece == WHITE_KING) ? WHITE_MOVED_ROOK : BLACK_MOVED_ROOK;
            b[move.to_row][0] = EMPTY;
        }
    }
}

int chess_is_in_check(chess_board_t b, int is_white) {
    // Encontra o rei
    int king_row = -1, king_col = -1;
    chess_Piece king = is_white ? WHITE_KING : BLACK_KING;
    chess_Piece moved_king = is_white ? WHITE_MOVED_KING : BLACK_MOVED_KING;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (b[i][j] == king || b[i][j] == moved_king) {
                king_row = i;
                king_col = j;
                break;
            }
        }
        if (king_row != -1) break;
    }

    if (king_row == -1) return 0;

    // Verifica ataques de peões
    int pawn_dir = is_white ? -1 : 1;
    chess_Piece enemy_pawn = is_white ? BLACK_PAWN : WHITE_PAWN;
    chess_Piece enemy_ep_pawn = is_white ? BLACK_EN_PASSANT_PAWN : WHITE_EN_PASSANT_PAWN;
    for (int dc = -1; dc <= 1; dc += 2) {
        if (is_valid_pos(king_row + pawn_dir, king_col + dc)) {
            chess_Piece p = b[king_row + pawn_dir][king_col + dc];
            if (p == enemy_pawn || p == enemy_ep_pawn) return 1;
        }
    }

    // Verifica ataques de cavalos
    chess_Piece enemy_knight = is_white ? BLACK_KNIGHT : WHITE_KNIGHT;
    int knight_moves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for (int i = 0; i < 8; i++) {
        int nr = king_row + knight_moves[i][0];
        int nc = king_col + knight_moves[i][1];
        if (is_valid_pos(nr, nc) && b[nr][nc] == enemy_knight) return 1;
    }

    // Verifica ataques retos (torre e rainha)
    chess_Piece enemy_rook = is_white ? BLACK_ROOK : WHITE_ROOK;
    chess_Piece enemy_moved_rook = is_white ? BLACK_MOVED_ROOK : WHITE_MOVED_ROOK;
    chess_Piece enemy_queen = is_white ? BLACK_QUEEN : WHITE_QUEEN;
    int straight_dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int d = 0; d < 4; d++) {
        for (int dist = 1; dist < 8; dist++) {
            int nr = king_row + straight_dirs[d][0] * dist;
            int nc = king_col + straight_dirs[d][1] * dist;
            if (!is_valid_pos(nr, nc)) break;
            chess_Piece p = b[nr][nc];
            if (p == EMPTY) continue;
            if (p == enemy_rook || p == enemy_moved_rook || p == enemy_queen) return 1;
            break;
        }
    }

    // Verifica ataques diagonais (bispo e rainha)
    chess_Piece enemy_bishop = is_white ? BLACK_BISHOP : WHITE_BISHOP;
    int diag_dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for (int d = 0; d < 4; d++) {
        for (int dist = 1; dist < 8; dist++) {
            int nr = king_row + diag_dirs[d][0] * dist;
            int nc = king_col + diag_dirs[d][1] * dist;
            if (!is_valid_pos(nr, nc)) break;
            chess_Piece p = b[nr][nc];
            if (p == EMPTY) continue;
            if (p == enemy_bishop || p == enemy_queen) return 1;
            break;
        }
    }

    // Verifica ataque do rei inimigo
    chess_Piece enemy_king = is_white ? BLACK_KING : WHITE_KING;
    chess_Piece enemy_moved_king = is_white ? BLACK_MOVED_KING : WHITE_MOVED_KING;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = king_row + dr;
            int nc = king_col + dc;
            if (is_valid_pos(nr, nc)) {
                chess_Piece p = b[nr][nc];
                if (p == enemy_king || p == enemy_moved_king) return 1;
            }
        }
    }

    return 0;
}

void chess_init_board(chess_board_t b) {
    // Peças pretas
    b[0][0] = BLACK_ROOK;
    b[0][1] = BLACK_KNIGHT;
    b[0][2] = BLACK_BISHOP;
    b[0][3] = BLACK_QUEEN;
    b[0][4] = BLACK_KING;
    b[0][5] = BLACK_BISHOP;
    b[0][6] = BLACK_KNIGHT;
    b[0][7] = BLACK_ROOK;

    for (int i = 0; i < 8; i++) {
        b[1][i] = BLACK_PAWN;
    }

    // Casas vazias
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            b[i][j] = EMPTY;
        }
    }

    // Peças brancas
    for (int i = 0; i < 8; i++) {
        b[6][i] = WHITE_PAWN;
    }

    b[7][0] = WHITE_ROOK;
    b[7][1] = WHITE_KNIGHT;
    b[7][2] = WHITE_BISHOP;
    b[7][3] = WHITE_QUEEN;
    b[7][4] = WHITE_KING;
    b[7][5] = WHITE_BISHOP;
    b[7][6] = WHITE_KNIGHT;
    b[7][7] = WHITE_ROOK;
}
