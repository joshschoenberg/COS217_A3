all: testsymtable testsymtablehash

testsymtable: testsymtable.o symtablelist.o
	gcc217 testsymtable.o symtablelist.o -o testsymtable

testsymtablehash: testsymtablehash.o symtablehash.o
	gcc217 testsymtablehash.o symtablehash.o -o testsymtablehash

testsymtable.o: testsymtable.c symtable.h
	gcc217 -c testsymtable.c

testsymtablehash.o: testsymtablehash.c symtable.h
	gcc217 -c testsymtablehash.c

symtablelist.o: symtablelist.c symtable.h
	gcc217 -c symtablelist.c

symtablehash.o: symtablehash.c symtable.h
	gcc217 -c symtablehash.c
