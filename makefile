# Compiler
CC = g++

# Source and output
SRC_DIR = src
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/login.c
OUT_DIR = build
OUT = $(OUT_DIR)/main.exe

# Include and library paths
INCLUDE = -Iinclude/
LIBS = -Llib/ -lraylib -lopengl32 -lgdi32 -lwinmm

# Force rebuild every time
.PHONY: all
all:
	$(CC) $(SRC) -o $(OUT) $(INCLUDE) $(LIBS)

# Clean rule
clean:
	del $(OUT)
