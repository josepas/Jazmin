LEX=flex
CCFLAGS=-g 


scanner: parser lexer
	gcc $(CCFLAGS) lex.yy.c grammar.tab.c -lbsd -lfl -o scanner

parser:
	bison -d -v grammar.y

lexer:
	flex lexer.l

clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.h grammar.output scanner