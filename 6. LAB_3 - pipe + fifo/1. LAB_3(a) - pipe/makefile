all: pipe

pipe: main.c
	clang -O0 $< -o $@

gdb: main.c
	clang -g3 -v -O0 $< -o $@

clear:
	rm *.o
