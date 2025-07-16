mkdir -p bin
tcc -o bin/chess src/chess.c src/main.c
tcc -shared -fPIC -o bin/libchess.so src/chess.c
# reduz o tamanho mas só vai funcionar se tiver rodando num os com /bin/sh
#gzexe bin/chess
#gzexe bin/libchess.so
rm -f bin/*~
./bin/chess