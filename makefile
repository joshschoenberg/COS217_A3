all: testsymtablelist testsymtablehash

testsymtablelist: testsymtablelist.o symtablelist.o
	gcc217 testsymtablelist.o symtablelist.o -o testsymtablelist

testsymtablehash: testsymtablehash.o symtablehash.o
	gcc217 testsymtablehash.o symtablehash.o -o testsymtablehash

testsymtablelist.o: testsymtablelist.c symtable.h
	gcc217 -c testsymtablelist.c

testsymtablehash.o: testsymtablehash.c symtable.h
	gcc217 -c testsymtablehash.c

symtablelist.o: symtablelist.c symtable.h
	gcc217 -c symtablelist.c

symtablehash.o: symtablehash.c symtable.h
	gcc217 -c symtablehash.c
