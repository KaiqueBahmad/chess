#include "chess.h"
#include <stdio.h>
#include <string.h>  // Para memset
#include <stdlib.h>  // Para rand() e srand()
#include <time.h>    // Para clock_t, clock(), time()

int possible_moves(board_t b, int i, int j, move_t moves[], int max_moves) {
    if (!valid(i,j) || !b[i][j]) return 0;
    
    signed char p = b[i][j], c = p > 0 ? 1 : -1, t = p < 0 ? -p : p;
    int m = 0, k, r, col, d;
    
    if (t == 6) { // Peão
        d = c > 0 ? 1 : -1;
        
        // Move forward
        if (valid(i+d,j) && !b[i+d][j]) {
            if (m < max_moves) {
                moves[m] = (move_t){i, j, i+d, j, p, 0};
                m++;
            }
            // Double move from starting position
            if (i == (c > 0 ? 1 : 6) && !b[i+2*d][j] && m < max_moves) {
                moves[m] = (move_t){i, j, i+2*d, j, p, 0};
                m++;
            }
        }
        
        // Capture diagonally left
        if (valid(i+d,j-1) && b[i+d][j-1] && (c*b[i+d][j-1] < 0) && m < max_moves) {
            moves[m] = (move_t){i, j, i+d, j-1, p, b[i+d][j-1]};
            m++;
        }
        
        // Capture diagonally right
        if (valid(i+d,j+1) && b[i+d][j+1] && (c*b[i+d][j+1] < 0) && m < max_moves) {
            moves[m] = (move_t){i, j, i+d, j+1, p, b[i+d][j+1]};
            m++;
        }
    }
    else if (t == 5) { // Cavalo
        static const signed char n[][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
        for (k=0; k<8 && m < max_moves; k++) {
            r = i + n[k][0]; col = j + n[k][1];
            if (valid(r,col) && (!b[r][col] || c*b[r][col] < 0)) {
                moves[m] = (move_t){i, j, r, col, p, b[r][col]};
                m++;
            }
        }
    }
    else if (t == 1) { // Rei
        for (r = i-1; r <= i+1 && m < max_moves; r++) {
            for (col = j-1; col <= j+1 && m < max_moves; col++) {
                if ((r != i || col != j) && valid(r,col) && (!b[r][col] || c*b[r][col] < 0)) {
                    moves[m] = (move_t){i, j, r, col, p, b[r][col]};
                    m++;
                }
            }
        }
    }
    else { // Torre, Bispo, Rainha
        static const signed char dir[][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        int start = t == 4 ? 0 : t == 3 ? 4 : 0;
        int end = t == 4 ? 4 : t == 3 ? 8 : 8;
        
        for (d = start; d < end && m < max_moves; d++) {
            for (k = 1; k < 8 && m < max_moves; k++) {
                r = i + dir[d][0] * k; col = j + dir[d][1] * k;
                if (!valid(r,col)) break;
                
                if (!b[r][col]) {
                    moves[m] = (move_t){i, j, r, col, p, 0};
                    m++;
                } else {
                    if (c*b[r][col] < 0) {
                        moves[m] = (move_t){i, j, r, col, p, b[r][col]};
                        m++;
                    }
                    break;
                }
            }
        }
    }
    
    return m;
}



int is_at_check(board_t b) {
    int kr[2] = {-1,-1}, kc[2] = {-1,-1}, i, j, r, col, k, d;
    
    // Encontrar reis
    for (i = 0; i < 64; i++) {
        if (b[i/8][i%8] == 1) { kr[0] = i/8; kc[0] = i%8; }
        if (b[i/8][i%8] == -1) { kr[1] = i/8; kc[1] = i%8; }
    }
    
    for (int color = 0; color < 2; color++) {
        if (kr[color] == -1) continue;
        int enemy = color ? 1 : -1;
        
        // Peões
        r = kr[color] - (enemy > 0 ? 1 : -1);
        if (valid(r, kc[color]-1) && b[r][kc[color]-1] == enemy*6) return enemy;
        if (valid(r, kc[color]+1) && b[r][kc[color]+1] == enemy*6) return enemy;
        
        // Cavalos
        static const signed char n[][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
        for (k = 0; k < 8; k++) {
            r = kr[color] + n[k][0]; col = kc[color] + n[k][1];
            if (valid(r,col) && b[r][col] == enemy*5) return enemy;
        }
        
        // Direções
        static const signed char dir[][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        for (d = 0; d < 8; d++) {
            for (k = 1; k < 8; k++) {
                r = kr[color] + dir[d][0]*k; col = kc[color] + dir[d][1]*k;
                if (!valid(r,col)) break;
                if (b[r][col]) {
                    if (k == 1 && b[r][col] == enemy) return enemy; // Rei
                    if (d < 4 && (b[r][col] == enemy*4 || b[r][col] == enemy*2)) return enemy; // Bispo/Rainha
                    if (d >= 4 && (b[r][col] == enemy*3 || b[r][col] == enemy*2)) return enemy; // Torre/Rainha
                    break;
                }
            }
        }
    }
    return 0;
}

// void generate_random_position(board_t board) {
// static int seeded = 0;
//   if (!seeded) {
//       struct timeval tv;
//       gettimeofday(&tv, NULL);
//       srand(tv.tv_sec * 1000000 + tv.tv_usec);  // Microsecond precision
//       seeded = 1;
//   }
//   memset(board, 0, sizeof(board_t));
  
//   // Sempre colocar os dois reis
//   board[rand() % 8][rand() % 8] = 1;  // Rei branco
//   board[rand() % 8][rand() % 8] = -1; // Rei preto
  
//   // Adicionar peças aleatórias (densidade ~25%)
//   for (int i = 0; i < 64; i++) {
//       if (rand() % 4 == 0 && board[i/8][i%8] == 0) {
//           int piece = (rand() % 5) + 2; // 2-6 (rainha, torre, bispo, cavalo, peão)
//           board[i/8][i%8] = (rand() % 2) ? piece : -piece;
//       }
//   }
// }

// void simple_benchmark(int num_positions) {
//   clock_t start, end;
//   long long total_moves = 0;
//   int check_count = 0;
  
//   printf("=== BENCHMARK COM %d POSIÇÕES ===\n", num_positions);
//   srand(time(NULL));
  
//   start = clock();
//   for (int test = 0; test < num_positions; test++) {
//       board_t board;
//       generate_random_position(board);
      
//       // Testar possible_moves em todas as casas
//       for (int i = 0; i < 8; i++) {
//           for (int j = 0; j < 8; j++) {
//               total_moves += possible_moves(board, i, j);
//           }
//       }
      
//       // Testar is_at_check
//       if (is_at_check(board)) check_count++;
//   }
//   end = clock();
  
//   double total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
  
//   printf("Tempo total: %.3f segundos\n", total_time);
//   printf("Posições/segundo: %.0f\n", num_positions / total_time);
//   printf("Calls possible_moves/segundo: %.0f\n", (num_positions * 64.0) / total_time);
//   printf("Total de movimentos calculados: %lld\n", total_moves);
//   printf("Posições em xeque: %d (%.1f%%)\n", check_count, (check_count * 100.0) / num_positions);
// }