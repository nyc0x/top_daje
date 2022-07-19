CC = gcc
OPTS = --std=gnu99 -Wall
FLAGS_UI = -lmenu -lncurses
ARGS_MAIN = src/parser/parser.c src/data_structures/proc_list_item.c src/util.c src/data_structures/linked_list.c src/test.c
ARGS_UI = src/parser/parser.c src/ui/ui.c src/data_structures/linked_list.c src/util.c src/data_structures/proc_list_item.c
BIN_MAIN = bin/finalTest
BIN_UI = bin/ui

main: $(ARGS_MAIN)
	$(CC) -o $(BIN_MAIN) $(ARGS_MAIN) $(OPTS)

ui: $(ARGS_UI)
	$(CC) -o $(BIN_UI) $(ARGS_UI) $(FLAGS_UI)

clean:
	rm -rf $(BIN_MAIN) $(BIN_UI)
