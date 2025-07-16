mkdir -p bin
tcc -o bin/chess src/chess.c src/main.c
tcc -shared -fPIC -o bin/libchess.so src/chess.c
gzexe bin/chess
rm -f bin/*~
./bin/chess