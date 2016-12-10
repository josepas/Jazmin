LEX=flex
CCFLAGS=-g -Wall

jaxmin: new_main.cpp grammar.tab.c lex.yy.c symbolTable.o typeTree.o utils.o ast.o list.o tac.o generator.o
	g++ new_main.cpp grammar.o lex.yy.o symbolTable.o typeTree.o utils.o ast.o list.o tac.o generator.o -lbsd -lfl -o $@


grammar.tab.c grammar.tab.h: grammar.y
	bison -d -v $^
	gcc -c grammar.tab.c -o grammar.o

lex.yy.c: lexer.l
	flex $^
	gcc -c lex.yy.c -o lex.yy.o

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

tac.o: utils/tac.h utils/tac.c
	gcc -g -c utils/tac.c -o tac.o

generator.o: CodeGenerator/generator.c CodeGenerator/generator.h
	g++ -g -c CodeGenerator/generator.c -o generator.o





clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.h grammar.output
	rm -f *.o *.gch
	rm -f jaxmin