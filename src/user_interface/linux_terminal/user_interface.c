#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Include the Interface Header
#include "../user_interface.h"

// --- ANSI Escape Codes for Terminal Colors and Styles ---
#define ANSI_COLOR_RESET    "\x1b[0m"
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_WHITE    "\x1b[37m"
#define ANSI_STYLE_BOLD     "\x1b[1m"
#define ANSI_STYLE_ITALIC   "\x1b[3m"

// Board Square Backgrounds
#define BG_HIGHLIGHT        "\x1b[44m"

// Piece Colors
#define PIECE_COLOR_WHITE   ANSI_COLOR_WHITE
#define PIECE_COLOR_BLACK   ANSI_COLOR_RED

// --- Utility Functions (Not in the header, only used internally) ---

// Converts chess_Piece enum to display character
static char piece_to_char(chess_Piece piece) {
    switch(piece) {
        case WHITE_KING:
        case WHITE_MOVED_KING: return 'K';
        case BLACK_KING:
        case BLACK_MOVED_KING: return 'k';
        case WHITE_QUEEN: return 'Q';
        case BLACK_QUEEN: return 'q';
        case WHITE_ROOK:
        case WHITE_MOVED_ROOK: return 'R';
        case BLACK_ROOK:
        case BLACK_MOVED_ROOK: return 'r';
        case WHITE_BISHOP: return 'B';
        case BLACK_BISHOP: return 'b';
        case WHITE_KNIGHT: return 'N';
        case BLACK_KNIGHT: return 'n';
        case WHITE_PAWN:
        case WHITE_EN_PASSANT_PAWN: return 'P';
        case BLACK_PAWN:
        case BLACK_EN_PASSANT_PAWN: return 'p';
        case EMPTY:
        default: return ' ';
    }
}

// Gets the string containing the background color code for a square
static const char* get_square_bg(int r, int c, int is_highlighted) {
    if (is_highlighted) {
        return BG_HIGHLIGHT;
    }
    return ANSI_COLOR_RESET; // No background color
}

// --- Implementation of Header Functions ---

void ui_display_welcome() {
    printf("%s%s------------------------------------%s\n", ANSI_STYLE_BOLD, ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    printf("%s%s| Platform Independent C Chess Engine |%s\n", ANSI_STYLE_BOLD, ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    printf("%s%s------------------------------------%s\n", ANSI_STYLE_BOLD, ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
}

void ui_display_possible_moves(chess_board_t board, chess_move_t moves[], int num_moves) {
    // Array to track which squares should be highlighted
    int highlight[8][8] = {{0}};
    for (int i = 0; i < num_moves; i++) {
        // Highlight the selected piece's square (source) and all destination squares
        highlight[moves[i].from_row][moves[i].from_col] = 1;
        highlight[moves[i].to_row][moves[i].to_col] = 1;
    }

    printf("\n   ");
    for (int c = 0; c < 8; c++) {
        printf(" %c ", 'a' + c); // Print file labels (a-h)
    }
    printf("\n");

    for (int r = 0; r < 8; r++) {
        printf(" %d ", 8 - r); // Print rank label (8-1)
        for (int c = 0; c < 8; c++) {
            chess_Piece piece = board[r][c];
            int is_white_piece = (piece > 0);

            // Convert enum to display character
            char piece_char = piece_to_char(piece);

            // Get background color string
            const char* bg = get_square_bg(r, c, highlight[r][c]);

            // Get foreground (piece) color string
            const char* fg = ANSI_COLOR_RESET;
            if (piece != EMPTY) {
                fg = is_white_piece ? PIECE_COLOR_WHITE : PIECE_COLOR_BLACK;
            }

            // Print the square with colors and the piece
            printf("%s%s%s%c%s ", bg, ANSI_STYLE_BOLD, fg, piece_char, ANSI_COLOR_RESET);
        }
        printf(" %d\n", 8 - r); // Print rank label again
    }
    printf("   ");
    for (int c = 0; c < 8; c++) {
        printf(" %c ", 'a' + c);
    }
    printf("\n");
}

void ui_draw_board(chess_board_t board) {
    // Standard drawing, no highlighting
    ui_display_possible_moves(board, NULL, 0);
}

void ui_display_turn(int is_white_turn) {
    const char* color = is_white_turn ? PIECE_COLOR_WHITE : PIECE_COLOR_BLACK;
    const char* player = is_white_turn ? "White" : "Black";
    printf("\n%s%s%s's turn.%s\n", ANSI_STYLE_BOLD, color, player, ANSI_COLOR_RESET);
}

void ui_announce_check(int is_white_turn) {
    const char* color = is_white_turn ? PIECE_COLOR_WHITE : PIECE_COLOR_BLACK;
    const char* player = is_white_turn ? "White" : "Black";
    printf("%s%sCHECK! %s is in check!%s\n", ANSI_STYLE_BOLD, ANSI_COLOR_RED, player, ANSI_COLOR_RESET);
}

void ui_announce_game_end(int is_checkmate, int is_white_turn) {
    const char* winner = is_white_turn ? "Black" : "White";
    printf("\n%s========================================%s\n", ANSI_STYLE_BOLD, ANSI_COLOR_RESET);
    if (is_checkmate) {
        printf("%s%sCHECKMATE! %s Wins!%s\n", ANSI_STYLE_BOLD, ANSI_COLOR_GREEN, winner, ANSI_COLOR_RESET);
    } else {
        printf("%s%sSTALEMATE! Game is a Draw.%s\n", ANSI_STYLE_BOLD, ANSI_COLOR_WHITE, ANSI_COLOR_RESET);
    }
    printf("%s========================================%s\n", ANSI_STYLE_BOLD, ANSI_COLOR_RESET);
}

chess_move_t ui_ask_select_piece(chess_board_t board, int is_white_turn) {
    char input[10];
    int valid = 0;
    int r, c;

    while (!valid) {
        printf("Enter piece to move (e.g., 'e2'): ");
        if (scanf("%9s", input) != 1) {
            // Handle scanf failure and clear input buffer
            while (getchar() != '\n');
            continue;
        }

        if (strlen(input) == 2 && input[0] >= 'a' && input[0] <= 'h' && input[1] >= '1' && input[1] <= '8') {
            c = input[0] - 'a';
            r = '8' - input[1];

            // Check if there's a piece at this location
            chess_Piece piece = board[r][c];
            if (piece == EMPTY) {
                printf("%sNo piece at %s. Try again.%s\n", ANSI_COLOR_RED, input, ANSI_COLOR_RESET);
                continue;
            }

            // Check if it's the correct color
            int is_white_piece = (piece > 0);
            if (is_white_piece != is_white_turn) {
                const char* expected = is_white_turn ? "white" : "black";
                printf("%sThat's not a %s piece. Try again.%s\n", ANSI_COLOR_RED, expected, ANSI_COLOR_RESET);
                continue;
            }

            valid = 1;
        } else {
            printf("%sInvalid format. Use algebraic notation (e.g., e2).%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        }
    }

    chess_move_t selected_piece_info;
    selected_piece_info.from_row = r;
    selected_piece_info.from_col = c;
    selected_piece_info.to_row = -1;
    selected_piece_info.to_col = -1;
    selected_piece_info.promoted_to = EMPTY;
    return selected_piece_info;
}

chess_move_t ui_ask_select_move(chess_move_t possible_moves[], int num_moves) {
    char input[10];
    int valid = 0;
    int r_dest, c_dest;

    while (!valid) {
        printf("Enter destination square (e.g., 'e4'): ");
        if (scanf("%9s", input) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (strlen(input) == 2 && input[0] >= 'a' && input[0] <= 'h' && input[1] >= '1' && input[1] <= '8') {
            c_dest = input[0] - 'a';
            r_dest = '8' - input[1];

            // Search possible_moves[] to find the exact move that matches this destination
            for (int i = 0; i < num_moves; i++) {
                if (possible_moves[i].to_row == r_dest && possible_moves[i].to_col == c_dest) {
                    chess_move_t selected_move = possible_moves[i];

                    // Check if this is a pawn promotion move
                    // Pawn reaches rank 0 (for white) or rank 7 (for black)
                    chess_Piece moving_piece = possible_moves[i].from_row; // This should be obtained from board
                    int is_promotion = (r_dest == 0 || r_dest == 7);

                    if (is_promotion && selected_move.promoted_to == EMPTY) {
                        // Ask for promotion piece
                        char promo_input[10];
                        int valid_promo = 0;

                        while (!valid_promo) {
                            printf("Promote to (Q/R/B/N): ");
                            if (scanf("%9s", promo_input) != 1) {
                                while (getchar() != '\n');
                                continue;
                            }

                            char promo_char = toupper(promo_input[0]);
                            int is_white = (possible_moves[i].from_row > 3); // Simple heuristic

                            // Determine if white or black based on destination rank
                            is_white = (r_dest == 0); // rank 0 means white pawn promoted

                            switch(promo_char) {
                                case 'Q':
                                    selected_move.promoted_to = is_white ? WHITE_QUEEN : BLACK_QUEEN;
                                    valid_promo = 1;
                                    break;
                                case 'R':
                                    selected_move.promoted_to = is_white ? WHITE_ROOK : BLACK_ROOK;
                                    valid_promo = 1;
                                    break;
                                case 'B':
                                    selected_move.promoted_to = is_white ? WHITE_BISHOP : BLACK_BISHOP;
                                    valid_promo = 1;
                                    break;
                                case 'N':
                                    selected_move.promoted_to = is_white ? WHITE_KNIGHT : BLACK_KNIGHT;
                                    valid_promo = 1;
                                    break;
                                default:
                                    printf("%sInvalid promotion piece. Choose Q, R, B, or N.%s\n",
                                           ANSI_COLOR_RED, ANSI_COLOR_RESET);
                            }
                        }
                    }

                    return selected_move;
                }
            }
            printf("%sInvalid move. The selected piece cannot move to %s.%s\n",
                   ANSI_COLOR_RED, input, ANSI_COLOR_RESET);

        } else {
            printf("%sInvalid format. Use algebraic notation (e.g., e4).%s\n",
                   ANSI_COLOR_RED, ANSI_COLOR_RESET);
        }
    }

    // Should be unreachable if the loop is correct
    chess_move_t dummy = {-1, -1, -1, -1, EMPTY};
    return dummy;
}
