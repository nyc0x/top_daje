CC = gcc

OPTS = --std=gnu99 -Wall

FLAGS_UI = -lncurses

ARGS_TEST = src/parser/parser.c src/data_structures/proc_list_item.c src/util.c src/data_structures/linked_list.c src/test.c

ARGS_UI = src/parser/parser.c src/ui/ui.c src/data_structures/linked_list.c src/util.c src/data_structures/proc_list_item.c

BIN_TEST = bin/finalTest

BIN_UI = bin/ui

./bin/ui: $(ARGS_UI)
	$(CC) -o $@ $(ARGS_UI) $(FLAGS_UI)

./bin/test test: $(ARGS_TEST)
	$(CC) -o $@ $< $(OPTS)

clean:
	rm -rf ./bin/ui
