# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS =

# Diretórios
SRC_DIR = src
CHESS_DIR = $(SRC_DIR)/chess
BIN_DIR = bin
OBJ_DIR = $(CHESS_DIR)

# Arquivos
TARGET = $(BIN_DIR)/chess
SOURCES = $(SRC_DIR)/main.c $(CHESS_DIR)/chess.c
OBJECTS = $(SRC_DIR)/main.o $(CHESS_DIR)/chess.o
HEADERS = $(CHESS_DIR)/chess.h

# Regra padrão
all: $(TARGET)

# Cria o executável
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build completo: $(TARGET)"

# Compila main.c
$(SRC_DIR)/main.o: $(SRC_DIR)/main.c $(HEADERS)
	$(CC) $(CFLAGS) -I$(CHESS_DIR) -c $(SRC_DIR)/main.c -o $(SRC_DIR)/main.o

# Compila chess.c
$(CHESS_DIR)/chess.o: $(CHESS_DIR)/chess.c $(HEADERS)
	$(CC) $(CFLAGS) -c $(CHESS_DIR)/chess.c -o $(CHESS_DIR)/chess.o

# Cria diretório bin se não existir
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Executa o programa
run: $(TARGET)
	./$(TARGET)

# Limpa arquivos compilados
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	@echo "Arquivos limpos"

# Limpa tudo incluindo o diretório bin
distclean: clean
	rm -rf $(BIN_DIR)
	@echo "Limpeza completa"

# Recompila tudo do zero
rebuild: clean all

# Mostra informações do projeto
info:
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Arquivos fonte: $(SOURCES)"
	@echo "Arquivos objeto: $(OBJECTS)"
	@echo "Executável: $(TARGET)"

# Marca regras que não são arquivos
.PHONY: all run clean distclean rebuild info
