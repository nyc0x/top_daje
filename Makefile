CC = gcc
OPTS = --std=gnu99 -Wall -Wno-unused-variable
FLAGS_UI = -g -O0 -lncurses
ARGS_TEST = src/parser/parser.c src/data_structures/proc_list_item.c src/util.c src/data_structures/linked_list.c src/test.c
ARGS_UI = src/parser/parser.c src/ui/ui.c src/data_structures/linked_list.c src/util.c src/data_structures/proc_list_item.c
BIN_TEST = bin/test
BIN_UI = bin/ui

ui: $(ARGS_UI)
	$(CC) -o $(BIN_UI) $(ARGS_UI) $(FLAGS_UI)

test: $(ARGS_TEST)
	$(CC) -o $(BIN_TEST) $(ARGS_TEST) $(OPTS)

clean:
	rm -rf $(BIN_TEST) $(BIN_UI) 
