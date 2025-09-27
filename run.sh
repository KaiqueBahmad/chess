mkdir -p bin

# compila o executável principal
tcc -I./src/chess \
    -o bin/chess \
    src/chess/chess.c \
    src/chess/chess-screen.c \
    src/main.c

# compila a biblioteca compartilhada
tcc -shared -fPIC -I./src/chess \
    -o bin/libchess.so \
    src/chess/chess.c \
    src/chess/chess-screen.c

# limpa arquivos temporários
rm -f bin/*~

# executa
./bin/chess
