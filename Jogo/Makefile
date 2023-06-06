CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic-errors -O0 -g 
LIBS = -lm -lcurses -I./includes 

ARGS = 

PRINCIPAL = jogo

SRC_DIR = src
OBJ_DIR = obj

# all .c files in the src/ directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# all .o files in the obj/ directory
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# target to link all .o files
$(PRINCIPAL): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $(PRINCIPAL) $(LIBS)

# recipe to compile all .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

# target to create the $(OBJ_DIR)/ directory if it does not exist
$(OBJ_DIR)/:
	mkdir -p $(OBJ_DIR)/

.PHONY: clean
.PHONY: run
.PHONY: debug 
.PHONY: mem

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(PRINCIPAL) 
	rm -f valgrind-out.txt
	clear

run: $(PRINCIPAL)
	./$(PRINCIPAL) $(ARGS)

debug: $(PRINCIPAL)
	gdb --args $(PRINCIPAL) $(ARGS)

mem:
	valgrind --leak-check=full --track-origins=yes --log-file=valgrind-out.txt --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --show-leak-kinds=all -s -- ./$(PRINCIPAL) $(ARGS)

