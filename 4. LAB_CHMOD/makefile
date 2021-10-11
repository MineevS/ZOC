all: test

test: main.c
	clang $< -o $@

gdb: main.c
	clang -g -v $< -o $@

clean: gdb
	rm $<
