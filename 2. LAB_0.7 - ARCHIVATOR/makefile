all: archiver

archiver: main.c
	clang $< -o $@

clean: archiver
	rm $<

gdb: main.c
	clang -g -v $< -o $@
 
