#include "symbolTable.h"


int main(int argc, char const *argv[]) {
	// probando la tabla de simbolos
    Symtable *current = createTable(NULL);

    insertTable(current, "a", 1, 2);
    insertTable(current, "cola", 1, 2);

    enterScope(&current);

    insertTable(current, "gusano", 1, 2);
    insertTable(current, "hashTabl", 1, 2);

    enterScope(&current);

    insertTable(current, "Hashy Potter", 1, 2);

    exitScope(&current);

    insertTable(current, "Hashy Potter", 1, 2);

    exitScope(&current);

    insertTable(current, "hashTabl", 1, 2);

    dumpTable(current);

    destroyTable(current);

	return 0;
}