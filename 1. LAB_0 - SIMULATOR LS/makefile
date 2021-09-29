all: ls

ls: main.c
	clang $< -o $@

gdb: main.c
	clang -g -v $< -o $@ -lc

clean: test
	rm $<

run: test
	./$<
