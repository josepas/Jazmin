LEX=flex

scanner: lexer parser
	gcc lex.yy.c grammar.tab.c -lbsd -o scanner

lexer:
	flex lexer.l

parser:
	bison -d grammar.y

clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.h scanner