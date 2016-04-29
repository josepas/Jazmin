LEX=flex

scanner: lexer parser
	gcc lex.yy.c grammar.tab.c -lbsd -lfl -o scanner

parser:
	bison -d grammar.y

lexer:
	flex lexer.l

clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.h grammar.output scanner