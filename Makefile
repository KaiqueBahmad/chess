# =================================================================
# PROJECT: Simple C Chess Game
# AUTHOR: Gemini
# DESCRIPTION: Makefile for building the chess game, allowing selection
#              of the User Interface implementation at compile time.
# =================================================================

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS =

# -------------------- UI SELECTION --------------------
# CHANGE THIS LINE to choose the UI you want to build.
# Options: linux (for linux_terminal), windows (for windows_gui), embedded (for embedded_lcd)
UI_TYPE = linux
# You can also set this on the command line: make UI_TYPE=windows

# -------------------- DIRECTORIES & PATHS --------------------
BIN_DIR = bin
SRC_DIR = src
CHESS_DIR = $(SRC_DIR)/chess
# FIX: Adjusted UI_DIR to correctly reflect the structure (it is inside src/)
UI_DIR = $(SRC_DIR)/user_interface

UI_LINUX_IMPL = $(UI_DIR)/linux_terminal
UI_WINDOWS_IMPL = $(UI_DIR)/windows_gui
UI_EMBEDDED_IMPL = $(UI_DIR)/embedded_lcd

# Select the correct UI implementation directory path
ifeq ($(UI_TYPE), linux)
    UI_IMPL_DIR = $(UI_LINUX_IMPL)
else ifeq ($(UI_TYPE), windows)
    UI_IMPL_DIR = $(UI_WINDOWS_IMPL)
else ifeq ($(UI_TYPE), embedded)
    UI_IMPL_DIR = $(UI_EMBEDDED_IMPL)
else
    $(error Invalid UI_TYPE specified. Choose linux, windows, or embedded.)
endif

# -------------------- FILES --------------------
TARGET = $(BIN_DIR)/chess

# Core Source Files
MAIN_C = $(SRC_DIR)/main.c
CHESS_C = $(CHESS_DIR)/chess.c

# Selected UI Source File
UI_C = $(UI_IMPL_DIR)/user_interface.c

# Combine all source files
SOURCES = $(MAIN_C) $(CHESS_C) $(UI_C)

# Object files are generated automatically from sources
OBJECTS = $(SOURCES:.c=.o)

# Headers (Dependencies for all .c files)
HEADERS = $(CHESS_DIR)/chess.h $(UI_DIR)/user_interface.h

# Include paths for the compiler (-I)
# -I$(CHESS_DIR) allows finding "chess.h"
# -I$(UI_DIR) allows finding "user_interface.h"
INCLUDE_FLAGS = -I$(CHESS_DIR) -I$(UI_DIR)

# -------------------- BUILD RULES --------------------

# Default target
all: $(TARGET)

# 1. Linking Rule: Create the final executable
# $^ refers to all prerequisites (OBJECTS)
# $@ refers to the target (TARGET)
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Build completo: $@ (UI selected: $(UI_TYPE))"

# 2. Compilation Rule (Pattern Rule): Compile any .c file into a .o file
# This single rule handles all source files (main, chess, and the selected UI)
# $< refers to the first prerequisite (the .c file)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $@

# -------------------- UTILITY TARGETS --------------------

# Executa o programa
run: $(TARGET)
	./$(TARGET)

# Limpa arquivos compilados
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	@echo "Arquivos objeto e executável limpos."

# Limpa tudo incluindo o diretório bin
distclean: clean
	rm -rf $(BIN_DIR)
	@echo "Limpeza completa (incluindo diretório bin)."

# Recompila tudo do zero
rebuild: clean all

# Mostra informações do projeto
info:
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "UI Selecionada: $(UI_TYPE)"
	@echo "Arquivo UI Fonte: $(UI_C)"
	@echo "Arquivos Objeto: $(OBJECTS)"
	@echo "Executável: $(TARGET)"
	@echo "Diretórios de Inclusão: $(INCLUDE_FLAGS)"

# Marca regras que não são arquivos
.PHONY: all run clean distclean rebuild info
