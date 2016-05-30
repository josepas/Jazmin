LEX=flex
CCFLAGS=-g -Wall

jaxmin: main.c grammar.tab.c lex.yy.c symbolTable.o
	gcc $(CCFLAGS) main.c grammar.tab.c symbolTable.o -lbsd -lfl -o $@

grammar.tab.c grammar.tab.h: grammar.y
	bison -d -v $^

lex.yy.c: lexer.l
	flex $^

symbolTable.o: SymbolTable/symbolTable.h SymbolTable/symbolTable.c
	gcc -c SymbolTable/symbolTable.c -o symbolTable.o


clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.h grammar.output
	rm -f *.o *.gch
	rm -f jaxmin