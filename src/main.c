#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chess/chess.h"

void print_board(board_t b) {
    printf("\n  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            char c = '.';
            Piece p = b[i][j];

            // Normaliza para exibição
            switch (p) {
                case BLACK_KING:
                case BLACK_MOVED_KING:
                    c = 'k'; break;
                case BLACK_QUEEN:
                    c = 'q'; break;
                case BLACK_ROOK:
                case BLACK_MOVED_ROOK:
                    c = 'r'; break;
                case BLACK_BISHOP:
                    c = 'b'; break;
                case BLACK_KNIGHT:
                    c = 'n'; break;
                case BLACK_PAWN:
                case BLACK_EN_PASSANT_PAWN:
                    c = 'p'; break;
                case WHITE_KING:
                case WHITE_MOVED_KING:
                    c = 'K'; break;
                case WHITE_QUEEN:
                    c = 'Q'; break;
                case WHITE_ROOK:
                case WHITE_MOVED_ROOK:
                    c = 'R'; break;
                case WHITE_BISHOP:
                    c = 'B'; break;
                case WHITE_KNIGHT:
                    c = 'N'; break;
                case WHITE_PAWN:
                case WHITE_EN_PASSANT_PAWN:
                    c = 'P'; break;
                default:
                    c = '.'; break;
            }
            printf("%c ", c);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n\n");
}

int parse_position(const char* pos, int* row, int* col) {
    if (strlen(pos) != 2) return 0;

    char col_char = tolower(pos[0]);
    char row_char = pos[1];

    if (col_char < 'a' || col_char > 'h') return 0;
    if (row_char < '1' || row_char > '8') return 0;

    *col = col_char - 'a';
    *row = 8 - (row_char - '0');

    return 1;
}

void print_piece_name(Piece p) {
    switch (p) {
        case BLACK_KING:
        case BLACK_MOVED_KING:
            printf("Rei preto"); break;
        case BLACK_QUEEN:
            printf("Rainha preta"); break;
        case BLACK_ROOK:
        case BLACK_MOVED_ROOK:
            printf("Torre preta"); break;
        case BLACK_BISHOP:
            printf("Bispo preto"); break;
        case BLACK_KNIGHT:
            printf("Cavalo preto"); break;
        case BLACK_PAWN:
        case BLACK_EN_PASSANT_PAWN:
            printf("Peão preto"); break;
        case WHITE_KING:
        case WHITE_MOVED_KING:
            printf("Rei branco"); break;
        case WHITE_QUEEN:
            printf("Rainha branca"); break;
        case WHITE_ROOK:
        case WHITE_MOVED_ROOK:
            printf("Torre branca"); break;
        case WHITE_BISHOP:
            printf("Bispo branco"); break;
        case WHITE_KNIGHT:
            printf("Cavalo branco"); break;
        case WHITE_PAWN:
        case WHITE_EN_PASSANT_PAWN:
            printf("Peão branco"); break;
        default:
            printf("Vazio"); break;
    }
}

int is_white_piece(Piece p) {
    return p > 0;
}

int is_black_piece(Piece p) {
    return p < 0;
}

int has_valid_moves(board_t b, int is_white_turn) {
    move_t moves[100];

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece p = b[i][j];
            if (p == EMPTY) continue;

            if ((is_white_turn && is_white_piece(p)) ||
                (!is_white_turn && is_black_piece(p))) {
                int count = possible_moves(b, i, j, moves, 100);
                if (count > 0) return 1;
            }
        }
    }
    return 0;
}

int main() {
    board_t board;
    char input[10];
    int white_turn = 1;
    int move_number = 1;

    printf("╔════════════════════════════════════╗\n");
    printf("║        JOGO DE XADREZ              ║\n");
    printf("║     Partida entre 2 jogadores      ║\n");
    printf("╚════════════════════════════════════╝\n\n");

    printf("Legenda:\n");
    printf("  Peças BRANCAS: K(Rei) Q(Rainha) R(Torre) B(Bispo) N(Cavalo) P(Peão)\n");
    printf("  Peças PRETAS:  k(rei) q(rainha) r(torre) b(bispo) n(cavalo) p(peão)\n\n");

    init_board(board);

    while (1) {
        print_board(board);

        // Verifica xeque
        int in_check = is_in_check(board, white_turn);
        if (in_check) {
            printf("⚠️  XEQUE! O rei %s está em xeque!\n\n",
                   white_turn ? "branco" : "preto");
        }

        // Verifica se há movimentos válidos
        if (!has_valid_moves(board, white_turn)) {
            if (in_check) {
                printf("\n╔════════════════════════════════════╗\n");
                printf("║          XEQUE-MATE!               ║\n");
                printf("║      %s VENCEU!        ║\n",
                       white_turn ? "PRETAS" : "BRANCAS");
                printf("╚════════════════════════════════════╝\n");
            } else {
                printf("\n╔════════════════════════════════════╗\n");
                printf("║            EMPATE!                 ║\n");
                printf("║          (Afogamento)              ║\n");
                printf("╚════════════════════════════════════╝\n");
            }
            break;
        }

        printf("Lance %d - Turno das %s\n", move_number,
               white_turn ? "BRANCAS" : "PRETAS");

        // Seleciona a peça
        int from_row, from_col;
        while (1) {
            printf("\nDigite a posição da peça que deseja mover (ex: e2) ou 'sair': ");
            if (scanf("%s", input) != 1) continue;

            if (strcmp(input, "sair") == 0) {
                printf("\nJogo encerrado.\n");
                return 0;
            }

            if (!parse_position(input, &from_row, &from_col)) {
                printf("❌ Posição inválida! Use formato como 'e2'.\n");
                continue;
            }

            Piece piece = board[from_row][from_col];

            if (piece == EMPTY) {
                printf("❌ Não há nenhuma peça nesta posição!\n");
                continue;
            }

            if ((white_turn && is_black_piece(piece)) ||
                (!white_turn && is_white_piece(piece))) {
                printf("❌ Esta peça não é sua! Escolha uma peça %s.\n",
                       white_turn ? "branca" : "preta");
                continue;
            }

            break;
        }

        // Mostra os movimentos possíveis
        move_t moves[100];
        int count = possible_moves(board, from_row, from_col, moves, 100);

        if (count == 0) {
            printf("\n❌ Esta peça não tem movimentos válidos! Escolha outra.\n");
            continue;
        }

        printf("\n✓ ");
        print_piece_name(board[from_row][from_col]);
        printf(" em %c%d\n", 'a' + from_col, 8 - from_row);
        printf("\nMovimentos possíveis:\n");

        for (int i = 0; i < count; i++) {
            printf("  %d) %c%d", i + 1,
                   'a' + moves[i].to_col, 8 - moves[i].to_row);

            Piece target = board[moves[i].to_row][moves[i].to_col];
            if (target != EMPTY) {
                printf(" (captura ");
                print_piece_name(target);
                printf(")");
            }

            if (moves[i].promoted_to != EMPTY) {
                printf(" (PROMOÇÃO)");
            }

            printf("\n");
        }

        // Seleciona o movimento
        int choice;
        while (1) {
            printf("\nEscolha o movimento (1-%d) ou 0 para cancelar: ", count);
            if (scanf("%d", &choice) != 1) {
                while (getchar() != '\n');
                printf("❌ Entrada inválida!\n");
                continue;
            }

            if (choice == 0) {
                printf("Movimento cancelado.\n");
                break;
            }

            if (choice < 1 || choice > count) {
                printf("❌ Escolha inválida! Digite um número entre 1 e %d.\n", count);
                continue;
            }

            // Executa o movimento
            make_move(board, moves[choice - 1]);

            printf("\n✓ Movimento executado: %c%d -> %c%d\n",
                   'a' + moves[choice - 1].from_col, 8 - moves[choice - 1].from_row,
                   'a' + moves[choice - 1].to_col, 8 - moves[choice - 1].to_row);

            white_turn = !white_turn;
            if (white_turn) move_number++;

            break;
        }

        if (choice == 0) continue;

        printf("\n");
        for (int i = 0; i < 50; i++) printf("─");
        printf("\n");
    }

    return 0;
}
