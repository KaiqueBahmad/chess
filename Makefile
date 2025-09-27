CC = tcc
CFLAGS = -I./src/chess -Os -Wall -s
LDFLAGS = 
BIN_DIR = bin
SRC_DIR = src/chess
MAIN = src/main.c
OBJS = $(SRC_DIR)/chess.c $(SRC_DIR)/chess-screen.c

all: $(BIN_DIR)/chess

$(BIN_DIR)/chess: $(OBJS) $(MAIN) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(MAIN) $(LDFLAGS)

$(BIN_DIR)/libchess.so: $(OBJS) | $(BIN_DIR)
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $(OBJS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/*

run: $(BIN_DIR)/chess
	./$(BIN_DIR)/chess
