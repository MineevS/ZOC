all: test

test: main.c
	clang $< -o $@

clean: test
	rm $<

gdb: main.c
	clang -g -v $< -o $@
 
