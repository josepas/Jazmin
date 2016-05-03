LEX=flex
CCFLAGS=-g 

main: scanner
	gcc $(CCFLAGS) main.c grammar.tab.c -lbsd -lfl -o jaxmin

scanner: parser lexer
	gcc -c $(CCFLAGS) lex.yy.c grammar.tab.c -lbsd -lfl 

parser:
	bison -d -v grammar.y

lexer:
	flex lexer.l

clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.h grammar.output *.o *.gch