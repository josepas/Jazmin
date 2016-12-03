#include "class.h"
#include "genny.h"
#include <set>


extern "C" {

	Generator* newGenerator() {
		return new Generator();
	}

	void deleteGenerator(Generator* v) {
		delete v;
	}

}