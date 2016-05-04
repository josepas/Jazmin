LEX=flex
CCFLAGS=-g -Wall

jaxmin: main.c grammar.tab.c lex.yy.c
	gcc $(CCFLAGS) main.c grammar.tab.c -lbsd -lfl -o $@

grammar.tab.c grammar.tab.h: grammar.y
	bison -d -v $^

lex.yy.c: lexer.l
	flex $^

clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.h grammar.output 
	rm -f *.o *.gch 
	rm -f jaxmin