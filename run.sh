mkdir -p bin
tcc -o bin/chess src/chess.c src/main.c
gzexe bin/chess
./bin/chess