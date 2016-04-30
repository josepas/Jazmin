#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char const *argv[]) {

	while ((c = getopt(argc, argv, "lp")) != -1) {
		switch (c) {
			case 'l':
				break;
			case 'p':
				break;
			case '?':
				if (optopt == 'blah')
					fprintf (stderr, "Option -%c requiere un argumento.\n", optopt);
				else if (isprint(optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return(EXIT_FAILURE);
			default:
				abort();
		}
	}
 	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);
	return 0;
z

	if (0) 
		return(EXIT_SUCCESS);
	else {
		return(EXIT_FAILURE);
	}
}