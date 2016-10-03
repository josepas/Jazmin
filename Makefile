LEX=flex
CCFLAGS=-g -Wall

jaxmin: main.c grammar.tab.c lex.yy.c symbolTable.o typeTree.o utils.o ast.o list.o
	gcc $(CCFLAGS) main.c grammar.tab.c symbolTable.o  typeTree.o utils.o ast.o list.o -lbsd -lfl -o $@

grammar.tab.c grammar.tab.h: grammar.y
	bison -d -v $^

lex.yy.c: lexer.l
	flex $^

symbolTable.o: SymbolTable/symbolTable.h SymbolTable/symbolTable.c
	gcc -g -c SymbolTable/symbolTable.c -o symbolTable.o

typeTree.o: TypeTree/typeTree.h TypeTree/typeTree.c
	gcc -g -c TypeTree/typeTree.c -o typeTree.o

utils.o: utils/utils.h utils/utils.c
	gcc -g -c utils/utils.c -o utils.o

list.o: List/DoublyLinkedList.h List/DoublyLinkedList.c
	gcc -g -c List/DoublyLinkedList.c -o list.o

ast.o: AST/ast.h AST/ast.c
	gcc -g -c AST/ast.c -o ast.o


clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.h grammar.output
	rm -f *.o *.gch
	rm -f jaxmin