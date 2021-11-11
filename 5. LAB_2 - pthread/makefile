all: test

test: main.c
	clang $< -lpthread -o $@

gdb: main.c
	clang -v -g $< -lpthread -o $@
