all: archiver

archiver: main_2.c
	clang $< -o $@

clean: archiver
	rm $<

gdb: main_2.c
	clang  -g -v $< -o $@
 
